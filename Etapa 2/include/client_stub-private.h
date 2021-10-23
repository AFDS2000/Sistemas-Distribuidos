#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H
#include "inet-private.h"
#include "client_stub.h"
#include <errno.h>

struct rtable_t
{
    int port;
    char *ip;
    int socket;
    struct sockaddr_in addr;
};

#endif