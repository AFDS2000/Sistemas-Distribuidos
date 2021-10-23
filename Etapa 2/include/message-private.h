#ifndef _TABLE_SKEL_H
#define _TABLE_SKEL_H

#include "inet.h"

int write_all(int sock, char *buf, int len);

int read_all(int sock, char *buf, int len);

#endif