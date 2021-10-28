#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H

#include <errno.h>
#include <inet-private.h>

struct rtable_t
{
    int port;
    char *ip;
    int socket;
    struct sockaddr_in addr;
};

#endif