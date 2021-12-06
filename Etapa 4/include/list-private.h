/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _LIST_PRIVATE_H
#define _LIST_PRIVATE_H

#include "list.h"

struct node_t {
    struct entry_t *value;
    struct node_t *next;
};

struct list_t {
    struct node_t *head;
    int length;
};

#endif
