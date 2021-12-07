#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <zookeeper/zookeeper.h>

#include "zookeeper_server-private.h"

#define ZDATALEN 1024 * 1024

typedef struct String_vector zoo_string;

static zhandle_t *zh;
static char *root_path = "/kvstore";
static char *primary = "/kvstore/primary";
static char *backup = "/kvstore/backup";
int is_connected;
struct zookeeper_data *zookeeper;
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

int create_node(zhandle_t *zh, const char *location, char *buffer, int buffer_len, int flags, char *new_path, int new_path_len)
{
    int zoo = zoo_create(zh, location, buffer, buffer_len, &ZOO_OPEN_ACL_UNSAFE, flags, new_path, new_path_len);

    return zoo;
}

static void child_watcher(zhandle_t *wzh, int type, int state, const char *zpath, void *watcher_ctx)
{

    int zoo_data_len = ZDATALEN;
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

                int a = zoo_create(zh, primary, zookeeper->backup_ip, strlen(zookeeper->backup_ip), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
                if (ZOK != a)
                {
                    fprintf(stderr, "Error creating znode from path %s!\n", primary);
                    exit(EXIT_FAILURE);
                }
                zoo_delete(zh, backup, -1);

                zookeeper->is_primary = 1;
                zookeeper->is_backup = 0;
                printf("\nbackup evolves to primary");
            }
            while (zookeeper->is_primary == 1 && ZNONODE == zoo_exists(zh, backup, 0, NULL))
            {
                sleep(1);
            }
        }
    }
}

int init_zookeeper(struct zookeeper_data *zoo_pointer, char *ip_port, char *port)
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
        char *localhost = "localhost:";
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
                    return -1;
                }
                return 0;
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
                        zoo_get(zh, primary, 0, zookeeper->primary_ip, &data_len, NULL);
                        if (ZOK != zoo_wget_children(zh, root_path, &child_watcher, watcher_ctx, zookeeper->children_list))
                        {
                            fprintf(stderr, "Error setting watch at %s!\n", root_path);
                        }
                        return 0;
                    }
                    else
                    {
                        return -1;
                    }
                }
            }
        }
        return 0;
    }
    return 2;
}

void zoo_destroy()
{
    zookeeper_close(zh);
    if (zookeeper)
    {
        if (zookeeper->children_list)
        {
            free(zookeeper->children_list);
        }
        if (zookeeper->backup_ip)
        {
            free(zookeeper->backup_ip);
        }
        if (zookeeper->primary_ip)
        {
            free(zookeeper->primary_ip);
        }
        free(zookeeper);
    }
}