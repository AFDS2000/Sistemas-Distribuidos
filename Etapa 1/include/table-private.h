#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "list.h"

struct table_t
{
    int size;
    struct table_element *list;
};

struct table_element
{
    struct list_t *list;
};
