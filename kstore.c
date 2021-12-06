#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <zookeeper/zookeeper.h>

/* ZooKeeper Znode Data Length (1MB, the max supported) */
#define ZDATALEN 1024 * 1024
static char *host_port = "localhost:2181";
static char *root_path = "/kvstore";
static zhandle_t *zh;
static int is_connected;
typedef struct String_vector zoo_string;
static int is_primary;
static int is_backup;
char *primary = "/kvstore/primary";
char *backup = "/kvstore/backup";
int zdata_len;
char *zdata_buf = NULL;
int new_path_len = 1024;
char *new_path;
static char *watcher_ctx = "ZooKeeper Data Watcher";
// data watcher estragado
static void data_watcher(zhandle_t *wzh, int type, int state, const char *zpath, void *watcher_ctx)
{
    char *zoo_data = malloc(ZDATALEN * sizeof(char));
    int zoo_data_len = ZDATALEN;
    if (state == ZOO_CONNECTED_STATE)
    {
        if (type == ZOO_CREATED_EVENT)
        {
            printf("\n print data watcher: ");
            /* Get the updated data and reset the watch */
            zoo_wget(wzh, primary, data_watcher, (void *)watcher_ctx, zoo_data, &zoo_data_len, NULL);
            for (int i = 0; i < zdata_len; i++)
            {
                char temp;
                memcpy(&temp, zdata_buf + i, 1);
                printf("%c", temp);
            }
        }
    }
    free(zoo_data);
}

int create_node(zhandle_t *zh, const char *location, char *buffer, int buffer_len, int flags, char *new_path, int new_path_len)
{
    int zoo = zoo_create(zh, location, buffer, buffer_len, &ZOO_OPEN_ACL_UNSAFE, flags, new_path, new_path_len);
    if (ZNODEEXISTS == zoo)
    {
        return zoo;
    }
    else
    {
        if (ZOK != zoo)
        {
            fprintf(stderr, "Error creating znode from path %s!\n", location);
            return zoo;
        }
        printf("%s doesn't exist! \nCreating.\n ", location);
        printf("%s created", location);
        return zoo;
    }
}

void print_get(zhandle_t *zh, const char *location, char *buffer, int *buffer_len)
{
    int zoo = zoo_get(zh, location, NULL, buffer, buffer_len, NULL);
    if (ZOK == zoo)
    {
        for (int i = 0; i < zdata_len; i++)
        {
            char temp;
            memcpy(&temp, zdata_buf + i, 1);
            printf("%c", temp);
        }
    }
}

void change_data(zhandle_t *zh, const char *location, char *buffer, int *buffer_len)
{
    int zoo = zoo_set(zh, location, buffer, *buffer_len, -1);
    printf("\nVerificar se a data foi alterada");
    print_get(zh, location, buffer, buffer_len);
}

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
// child watcher ve se foi removido primary a adiciona um novo
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
            if (ZNONODE == zoo_exists(zh, primary, 0, NULL) && is_backup == 1)
            {
                zdata_buf = (char *)calloc(1, ZDATALEN * sizeof(char));
                zdata_len = ZDATALEN * sizeof(char);
                int a = zoo_create(zh, primary, "127.0.0.2", 10, &ZOO_OPEN_ACL_UNSAFE, ZOO_EPHEMERAL, new_path, new_path_len);
                if (ZOK != a)
                {
                    fprintf(stderr, "Error creating znode from path %s!\n", primary);
                    exit(EXIT_FAILURE);
                }
                zoo_delete(zh, backup, NULL);

                free(zdata_buf);

                is_primary = 1;
                is_backup = 0;
                printf("\nbackup evolves to primary");
            }
        }
    }
    free(children_list);
}

int main(void)
{
    zoo_string *children_list = NULL;
    new_path = malloc(new_path_len);
    struct tm *local;
    zh = zookeeper_init(host_port, connection_watcher, 2000, 0, 0, 0);

    if (zh == NULL)
    {
        fprintf(stderr, "Error connecting to ZooKeeper server[%d]!\n", errno);
        exit(EXIT_FAILURE);
    }
    sleep(1);
    if (is_connected)
    {

        char *data_root = "oi";

        int nodeRoot = create_node(zh, root_path, data_root, 3, 0, new_path, new_path_len);
        if (ZOK != nodeRoot && ZNODEEXISTS != nodeRoot)
        {
            fprintf(stderr, "Error creating znode from path %s!\n", primary);
            exit(EXIT_FAILURE);
        }

        char *data_primary = "127.0.0.1:19";
        int nodePrimary = create_node(zh, primary, data_primary, 10, ZOO_EPHEMERAL, new_path, new_path_len);
        if (ZOK != nodePrimary && ZNODEEXISTS != nodePrimary)
        {
            fprintf(stderr, "Error creating znode from path %s!\n", primary);
            exit(EXIT_FAILURE);
        }
        else
        {
            if (ZOK == nodePrimary)
            {
                is_primary = 1;

                while (1)
                {
                    printf("\n primary: %d", is_primary);
                }
            }
            else
            {
                char *data_backup = "127.0.0.1:20";
                int nodeBackup = create_node(zh, backup, data_backup, 10, ZOO_EPHEMERAL, new_path, new_path_len);
                if (ZOK != nodeBackup && ZNODEEXISTS != nodeBackup)
                {
                    fprintf(stderr, "Error creating znode from path %s!\n", primary);
                    exit(EXIT_FAILURE);
                }
                else
                {
                    if (ZOK == nodeBackup)
                    {
                        is_backup = 1;

                        children_list = (zoo_string *)malloc(sizeof(zoo_string));

                        if (ZOK != zoo_wget_children(zh, root_path, &child_watcher, watcher_ctx, children_list))
                        {
                            fprintf(stderr, "Error setting watch at %s!\n", root_path);
                        }

                        zdata_buf = (char *)malloc(ZDATALEN * sizeof(char));
                        zdata_len = ZDATALEN * sizeof(char);

                        if (ZOK != zoo_wget(zh, primary, data_watcher, watcher_ctx, zdata_buf, &zdata_len, NULL))
                        {
                            fprintf(stderr, "Error setting watch at %s!\n", backup);
                        }

                        while (1)
                        {
                            if (is_backup)
                                printf("\nbackup: %d", is_backup);
                            else
                            {
                                char *zdata_buf1 = (char *)calloc(1, ZDATALEN * sizeof(char));
                                zdata_len = ZDATALEN * sizeof(char);
                                int zoo = zoo_get(zh, primary, NULL, zdata_buf1, &zdata_len, NULL);
                                char *zdata_buf2 = (char *)calloc(1, ZDATALEN * sizeof(char));
                                zoo = zoo_get(zh, backup, NULL, zdata_buf2, &zdata_len, NULL);
                                int o = 0;
                            }
                        }
                    }
                    else
                    {
                        exit(0);
                    }
                }
            }
        }
    }
    else
    {
        exit(0);
    }
}
