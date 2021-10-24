/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _TABLE_PRIVATE_H
#define _TABLE_PRIVATE_H

struct table_t {
    int size;
    int count;
    struct list_t **items;
};

//Função que calcula o hash da key passada por argumento
int hash(char *key, int n);

#endif