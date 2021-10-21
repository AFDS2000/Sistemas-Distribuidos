#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table_skel-private.h"

int countNumbers(int n)
{
    int count = 0;
    do
    {
        n /= 10;
        count++;
    } while (n != 0);

    return count;
}

void get_table_size(MessageT *msg, struct table_t *table)
{
    int size, count = 0;

    msg->opcode += 1;
    msg->c_type = 50;

    size = table_size(table);

    if (msg->data == NULL)
        msg->data = malloc(count + 1);

    sprintf(msg->data, "%d", size);
    msg->data_size = strlen(msg->data) + 1;
    return 0;
}

void del_entry(MessageT *msg, struct table_t *table)
{
    char *key = strdup(msg->data);

    int err = table_del(table, key);
    if (err == -1)
    {
        msg->opcode = 99;
        msg->c_type = 70;
    }
    else
    {
        msg->opcode += 1;
        msg->c_type = 70;
    }

    msg->data = NULL;
    msg->data_size = 0;
}

void get_entry(MessageT *msg, struct table_t *table)
{
    char *key = strdup(msg->data);
    struct data_t *data = table_get(table, key);

    msg->opcode += 1;
    msg->c_type = 20;

    if (data == NULL)
    {
        msg->data = NULL;
        msg->data_size = 0;
    }
    else
    {
        msg->data = data->data;
        msg->data_size = data->datasize;
    }
}

void put_entry(msg, table) {

}

void get_keys(msg, table) {
    char **keys = table_get_keys(table);

}

void table_to_string(msg, table) {
    
}