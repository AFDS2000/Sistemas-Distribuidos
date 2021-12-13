/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <zookeeper/zookeeper.h>

#include "table.h"
#include "inet-private.h"
#include "table_skel.h"
#include "network_server.h"
#include "zookeeper_server-private.h"

#define ZDATALEN 1024 * 1024

typedef struct String_vector zoo_string;

static zhandle_t *zh;
static char *root_path = "/kvstore";
static char *primary = "/kvstore/primary";
static char *backup = "/kvstore/backup";
int is_connected;
static struct zookeeper_data *zookeeper;
static char *watcher_ctx = "ZooKeeper Data Watcher";

void connection_watcher(zhandle_t *zzh, int type, int state, const char *path, void *context)
{
    if (type == ZOO_SESSION_EVENT)
    {
        if (state == ZOO_CONNECTED_STATE)
        {
            is_connected = 1;
        }
        else
        {
            is_connected = 0;
        }
    }
}

static void child_watcher(zhandle_t *wzh, int type, int state, const char *zpath, void *watcher_ctx)
{

    if (state == ZOO_CONNECTED_STATE)
    {
        if (type == ZOO_CHILD_EVENT)
        {
            /* Get the updated children and reset the watch */
            if (ZOK != zoo_wget_children(zh, root_path, child_watcher, watcher_ctx, zookeeper->children_list))
            {
                fprintf(stderr, "Error setting watch at %s!\n", root_path);
            }
            // torna backup em primary mas deve ser feito em watcher
            if (ZNONODE == zoo_exists(zh, primary, 0, NULL) && zookeeper->is_backup == 1)
            {
                zoo_delete(zh, backup, -1);

                if (ZOK != zoo_create(zh, primary, zookeeper->backup_ip, strlen(zookeeper->backup_ip), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0))
                {
                    fprintf(stderr, "Error creating znode from path %s!\n", primary);
                    exit(EXIT_FAILURE);
                }
                free(zookeeper->primary_ip);
                zookeeper->primary_ip = zookeeper->backup_ip;
                zookeeper->backup_ip = NULL;
                zookeeper->is_primary = 1;
                zookeeper->is_backup = 0;
                printf("\nbackup evolves to primary");
            }
            //Se for servidor primario e o backup tiver saido, nao aceita mais pedidos de
            //escrita dos clientes ate que volte a haver backup. Volta a ativar o watch.
            if (zookeeper->is_primary == 1 && ZNONODE == zoo_exists(zh, backup, 0, NULL))
            {
                zookeeper->write_enable = 0;
            }
            if (zookeeper->is_primary == 1 && ZOK == zoo_exists(zh, backup, 0, NULL))
            {
                if (zookeeper->backup_ip == NULL)
                {
                    zookeeper->backup_ip = calloc(1, ZDATALEN);
                }
                int len = ZDATALEN;
                zoo_get(zh, backup, 0, zookeeper->backup_ip, &len, NULL);
                zookeeper->write_enable = 1;
                sleep(2);
                //lixo de replicacao
                int sock = backup_client();
                if (sock != -1)
                {
                    static MessageT msg;
                    message_t__init(&msg);
                    msg.opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
                    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;

                    invoke(&msg);
                    for (int i = 0; i < msg.n_keys; i++)
                    {
                        static MessageT msg_enviar;
                        message_t__init(&msg_enviar);

                        msg_enviar.opcode = MESSAGE_T__OPCODE__OP_GET;
                        msg_enviar.c_type = MESSAGE_T__C_TYPE__CT_NONE;
                        msg_enviar.keys = malloc(sizeof(char *));
                        msg_enviar.keys[0] = strdup(msg.keys[i]);

                        msg_enviar.n_keys = 1;
                        invoke(&msg_enviar);
                        msg_enviar.opcode = MESSAGE_T__OPCODE__OP_PUT;

                        uint8_t *buf = NULL;
                        int len = message_t__get_packed_size(&msg_enviar);
                        buf = malloc(len);
                        message_t__pack(&msg_enviar, buf);
                        MessageT *msg_nova = message_t__unpack(NULL, len, buf);

                        network_send(sock, msg_nova);
                        free(buf);
                    }

                    table_free_keys(msg.keys);
                }
            }
        }
    }
}

struct zookeeper_data *init_zookeeper(struct zookeeper_data *zoo_pointer, char *ip_port, char *port)
{
    zh = zookeeper_init(ip_port, connection_watcher, 2000, 0, 0, 0);
    int data_len = ZDATALEN * sizeof(char);
    if (zh == NULL)
    {
        fprintf(stderr, "Error connecting to ZooKeeper server[%d]!\n", errno);
        exit(EXIT_FAILURE);
    }
    sleep(1);
    zookeeper = calloc(1, sizeof(struct zookeeper_data));
    zookeeper->is_backup = 0;
    zookeeper->is_primary = 0;
    zookeeper->write_enable = 0;
    zookeeper->backup_ip = NULL;
    zookeeper->primary_ip = NULL;
    zookeeper->children_list = (zoo_string *)malloc(sizeof(zoo_string));
    zoo_pointer = zookeeper;

    if (is_connected)
    {

        int nodeRoot = zoo_create(zh, root_path, NULL, -1, &ZOO_OPEN_ACL_UNSAFE, 0, NULL, 0);
        if (ZOK != nodeRoot && ZNODEEXISTS != nodeRoot)
        {
            fprintf(stderr, "Error creating znode from path %s!\n", primary);
            exit(EXIT_FAILURE);
        }
        char *localhost = "127.0.0.1:";
        char *temp_ip = calloc(1, ZDATALEN);
        strcat(temp_ip, localhost);
        strcat(temp_ip, port);

        //caso quando existe backup e nao primary
        int backupExists = zoo_exists(zh, backup, 0, NULL);
        int primaryExists = zoo_exists(zh, primary, 0, NULL);
        while (primaryExists == ZNONODE && backupExists == ZOK)
        {
            sleep(1);
            primaryExists = zoo_exists(zh, primary, 0, NULL);
            backupExists = zoo_exists(zh, backup, 0, NULL);
        }
        int nodePrimary = zoo_create(zh, primary, temp_ip, strlen(temp_ip), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
        if (ZOK != nodePrimary && ZNODEEXISTS != nodePrimary)
        {
            fprintf(stderr, "Error creating znode from path %s!\n", primary);
            exit(EXIT_FAILURE);
        }
        else
        {
            if (ZOK == nodePrimary)
            {
                zookeeper->primary_ip = temp_ip;
                zookeeper->is_primary = 1;
                if (ZOK != zoo_wget_children(zh, root_path, &child_watcher, watcher_ctx, zookeeper->children_list))
                {
                    fprintf(stderr, "Error setting watch at %s!\n", root_path);
                    return NULL;
                }
                return zookeeper;
            }
            else
            {
                int nodeBackup = zoo_create(zh, backup, temp_ip, strlen(temp_ip), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
                if (ZOK != nodeBackup && ZNODEEXISTS != nodeBackup)
                {
                    fprintf(stderr, "Error creating znode from path %s!\n", primary);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    if (ZOK == nodeBackup)
                    {
                        zookeeper->backup_ip = temp_ip;
                        zookeeper->is_backup = 1;
                        zookeeper->primary_ip = calloc(1, ZDATALEN);
                        zookeeper->write_enable = 1;
                        zoo_get(zh, primary, 0, zookeeper->primary_ip, &data_len, NULL);
                        if (ZOK != zoo_wget_children(zh, root_path, &child_watcher, watcher_ctx, zookeeper->children_list))
                        {
                            fprintf(stderr, "Error setting watch at %s!\n", root_path);
                        }
                        return zookeeper;
                    }
                    else
                    {
                        return NULL;
                    }
                }
            }
        }
        return zookeeper;
    }
    return NULL;
}

void zoo_destroy()
{
    if (zookeeper)
    {
        if (zookeeper->children_list != NULL)
        {
            free(zookeeper->children_list);
        }
        if (zookeeper->backup_ip != NULL)
        {
            free(zookeeper->backup_ip);
        }
        if (zookeeper->primary_ip != NULL)
        {
            free(zookeeper->primary_ip);
        }
        free(zookeeper);
    }
    zookeeper_close(zh);
}

int backup_client()
{

    if (zookeeper->is_primary == 1 && ZOK == zoo_exists(zh, backup, 0, NULL))
    {
        struct sockaddr_in serv_addr;
        int sock = 0;

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {
            perror("Erro ao criar socket TCP");
            return -1;
        }
        serv_addr.sin_family = AF_INET;
        char *addr_port = strdup(zookeeper->backup_ip);
        char *token;
        const char s[2] = ":";
        token = strtok(addr_port, s);
        char *ip = token;
        token = strtok(NULL, s);
        int port = atoi(token);
        // família de endereços
        serv_addr.sin_port = htons(port); // Porta TCP
        if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) < 1)
        {
            printf("Erro ao converter IP\n");
            close(sock);
            return -1;
        }
        if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            perror("Erro ao conectar-se ao servidor");
            close(sock);
            return -1;
        }
        return sock;
    }
    else
    {
        return -1;
    }
}