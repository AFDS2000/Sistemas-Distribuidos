/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _ZOOKEEPER_SERVER_PRIVATE_H
#define _ZOOKEEPER_SERVER_PRIVATE_H

struct zookeeper_data
{
    int is_primary;
    int is_backup;
    char *primary_ip;
    char *backup_ip;
    struct String_vector *children_list;
};
int init_zookeeper(struct zookeeper_data *, char *, char *);

void zoo_destroy();
#endif