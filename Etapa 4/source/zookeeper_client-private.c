#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <zookeeper/zookeeper.h>

#include "client_stub.h"
#include "zookeeper_client-private.h"

#define ZDATALEN 1024 * 1024

typedef struct String_vector zoo_string;
extern struct rtable_t *table;
extern int primary_exists;

char *root_path = "/kvstore";
char *primary = "/kvstore/primary";
zhandle_t *zh;
static int is_connected;

void connection_watcher(zhandle_t *zzh, int type, int state, const char *path, void *context)
{
    if (type == ZOO_SESSION_EVENT)
    {
        if (state == ZOO_CONNECTED_STATE)
            is_connected = 1;
        else
            is_connected = 0;
    }
}

// child watcher ve se foi removido primary a adiciona um novo
static void child_watcher(zhandle_t *wzh, int type, int state, const char *zpath, void *watcher_ctx)
{
    zoo_string *children_list = (zoo_string *)malloc(sizeof(zoo_string));

    /* Get the updated children and reset the watch */
    if (ZOK != zoo_wget_children(zh, root_path, child_watcher, watcher_ctx, children_list))
    {
        fprintf(stderr, "Error setting watch at %s!\n", root_path);
    }

    if (state == ZOO_CONNECTED_STATE)
    {
        if (type == ZOO_CHILD_EVENT)
        {
            if (ZNONODE == zoo_exists(zh, primary, 0, NULL) && primary_exists == 1)
            {
                primary_exists = 0;
                rtable_disconnect(table);
                table->ip = NULL;
                table->port = 0;
            }
            else if (ZOK == zoo_exists(zh, primary, 0, NULL) && primary_exists == 0)
            {
                int data_len = ZDATALEN * sizeof(char);
                primary_exists = 1;
                char *ip_port = malloc(data_len);
                zoo_get(zh, primary, 0, ip_port, &data_len, NULL);
                table = rtable_connect(ip_port);
            }
        }
    }
    free(children_list);
}

void init_zookeeper(const char *ip_port)
{
    int data_len = ZDATALEN * sizeof(char);
    if ((zh = zookeeper_init(ip_port, connection_watcher, 2000, 0, 0, 0)) == NULL)
    {
        fprintf(stderr, "Error connecting to ZooKeeper server[%d]!\n", errno);
        exit(EXIT_FAILURE);
    }
    sleep(1);

    if (is_connected == 1)
    {
        zoo_string *children_list = (zoo_string *)malloc(sizeof(zoo_string));

        if (ZOK != zoo_wget_children(zh, root_path, &child_watcher, NULL, children_list))
        {
            fprintf(stderr, "Error setting watch at %s!\n", root_path);
            return;
        }

        if (ZOK == zoo_exists(zh, primary, 0, NULL))
        {
            char *ip_port = calloc(1, data_len);
            zoo_get(zh, primary, 0, ip_port, &data_len, NULL);
            table = rtable_connect(ip_port);
            primary_exists = 1;
            free(ip_port);
        }
        else
        {
            primary_exists = 0;
        }

        free(children_list);
    }
    return;
}

void zoo_destroy()
{
    zookeeper_close(zh);
}