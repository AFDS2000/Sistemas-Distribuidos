#include "zookeeper_server-private.h"
#include <zookeeper/zookeeper.h>
#define ZDATALEN 1024 * 1024
typedef struct String_vector zoo_string;

static zhandle_t *zh;
static char *root_path = "/kvstore";
static char *primary = "/kvstore/primary";
static char *backup = "/kvstore/backup";
int is_connected;
struct zookeeper_data *zookeeper;

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
    zoo_string *children_list = (zoo_string *)malloc(sizeof(zoo_string));
    int zoo_data_len = ZDATALEN;
    if (state == ZOO_CONNECTED_STATE)
    {
        if (type == ZOO_CHILD_EVENT)
        {
            /* Get the updated children and reset the watch */
            if (ZOK != zoo_wget_children(zh, root_path, child_watcher, watcher_ctx, children_list))
            {
                fprintf(stderr, "Error setting watch at %s!\n", root_path);
            }
            // torna backup em primary mas deve ser feito em watcher
            if (ZNONODE == zoo_exists(zh, primary, 0, NULL) && zookeeper->is_backup == 1)
            {

                int a = zoo_create(zh, primary, zookeeper->backup_ip, sizeof(zookeeper->backup_ip), &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, NULL, 0);
                if (ZOK != a)
                {
                    fprintf(stderr, "Error creating znode from path %s!\n", primary);
                    exit(EXIT_FAILURE);
                }
                zoo_delete(zh, backup, NULL);

                zookeeper->is_primary = 1;
                zookeeper->is_backup = 0;
                printf("\nbackup evolves to primary");
            }
        }
    }
    free(children_list);
}

int init_zookeeper(struct zookeeper_data *zoo_pointer, char *ip_port, char *port)
{
    zh = zookeeper_init(ip_port, connection_watcher, 2000, 0, 0, 0);
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
    zoo_pointer = zookeeper;

    if (is_connected)
    {

        int nodeRoot = create_node(zh, root_path, NULL, -1, 0, NULL, 0);
        if (ZOK != nodeRoot && ZNODEEXISTS != nodeRoot)
        {
            fprintf(stderr, "Error creating znode from path %s!\n", primary);
            exit(EXIT_FAILURE);
        }
        char *temp = "localhost:";
        int nodePrimary = create_node(zh, primary, strcat(temp, port), sizeof(temp), ZOO_EPHEMERAL, NULL, 0);
        if (ZOK != nodePrimary && ZNODEEXISTS != nodePrimary)
        {
            fprintf(stderr, "Error creating znode from path %s!\n", primary);
            exit(EXIT_FAILURE);
        }

        zoo_string *children_list = NULL;

        return 0;
    }
}