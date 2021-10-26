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

int countKeys(char **keys)
{
    int i = 0;
    while (keys[i])
    {
        i++;
    }
    return i;
}

void get_table_size(MessageT *msg, struct table_t *table)
{
    msg->opcode += 1;
    msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;

    int count = countNumbers(table_size(table));
    msg->table_size = count;
}

void del_entry(MessageT *msg, struct table_t *table)
{
    char *key = strdup(msg->keys[0]->key);

    int err = table_del(table, key);
    if (err == -1)
    {
        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }
    else
    {
        msg->opcode += 1;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }
}

void get_entry(MessageT *msg, struct table_t *table)
{
    char *key = strdup(msg->keys[0]->key);
    struct data_t *data = table_get(table, key);

    if (data == NULL)
    {
        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }
    else
    {
        msg->opcode += 1;
        msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;

        ProtobufCBinaryData data_temp;
        data_temp.len = data->datasize;
        data_temp.data = malloc(data->datasize);
        memcpy(data_temp.data, data->data, data->datasize);
        msg->data = data_temp;
    }
}

void put_entry(MessageT *msg, struct table_t *table)
{
    char *key = strdup(msg->keys[0]->key);
    struct data_t *data = malloc(sizeof(struct data_t));

    data->datasize = msg->data.len;
    data->data = msg->data.data;

    int erro = table_put(table, key, data);

    if (erro == -1)
    {
        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }
    else
    {
        msg->opcode += 1;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }
}

void get_keys(MessageT *msg, struct table_t *table)
{
    char **keys = table_get_keys(table);

    int numKeys = countKeys(keys);
    msg->n_keys = numKeys;
    msg->keys = malloc(numKeys * sizeof(MessageT__Key *));

    // Inicia key_temp
    MessageT__Key *key_temp = malloc(numKeys * sizeof(MessageT__Key));

    // loop para preencher as entries de msg
    // (necessario passsar os valores de cada entries[i] para cada msg.entries[i],
    // pois as estuturas de entries e de msg.entries s~ao diferentes)
    for (int i = 0; i < numKeys; i++)
    {
        sdmessage__entry__init(&key_temp[i]);

        key_temp[i].key = malloc(strlen(keys[i]) + 1);
        strcpy(key_temp[i].key, keys[i]);

        msg->keys[i] = &key_temp[i];
    }

    msg->opcode += 1; 
    msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
}

void table_to_string(MessageT *msg, struct table_t *table)
{
}