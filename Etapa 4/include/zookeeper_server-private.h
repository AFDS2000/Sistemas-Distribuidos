/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _ZOOKEEPER_SERVER_PRIVATE_H
#define _ZOOKEEPER_SERVER_PRIVATE_H

#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <zookeeper/zookeeper.h>

struct zookeeper_data
{
    int is_primary;
    int is_backup;
    char *primary_ip;
    char *backup_ip;
};
int init_zookeeper(struct zookeeper_data *, char *, char *);
#endif