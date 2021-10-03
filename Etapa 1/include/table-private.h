#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

#include "list.h"

struct table_t {
    int size;
    int count;
    struct list_t **items;
};

//Função que calcula o hash da key passada por argumento
int hash(char *key, int n);

#endif

