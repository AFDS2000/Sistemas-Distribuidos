/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _MESSAGE_PRIVATE_H
#define _MESSAGE_PRIVATE_H

#include "inet-private.h"

int write_all(int sock, uint8_t *buf, int len);

int read_all(int sock, uint8_t *buf, int len);

#endif