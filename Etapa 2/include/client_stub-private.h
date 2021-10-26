#ifndef _CLIENT_STUB_PRIVATE_H
#define _CLIENT_STUB_PRIVATE_H
#include "inet-private.h"
#include "client_stub.h"
#include <errno.h>
#include "entry.h"
#include "data.h"
struct rtable_t
{
    int port;
    char *ip;
    int socket;
    struct sockaddr_in addr;
};

#endif