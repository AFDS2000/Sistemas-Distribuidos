/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include <errno.h>
#include "inet-private.h"

struct rtable_t
{
    int port;
    char *ip;
    int socket;
    struct sockaddr_in addr;
};

#endif