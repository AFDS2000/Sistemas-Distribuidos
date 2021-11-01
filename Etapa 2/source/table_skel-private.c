#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table_skel-private.h"

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
    msg->table_size = table_size(table);
}

void del_entry(MessageT *msg, struct table_t *table)
{
    if (table_del(table, msg->keys[0]) == -1)
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
    if (msg->keys[0] == NULL)
    {
        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        return;
    }

    struct data_t *data = table_get(table, msg->keys[0]);

    msg->opcode += 1;
    msg->c_type = MESSAGE_T__C_TYPE__CT_VALUE;

    if (data == NULL)
    {
        ProtobufCBinaryData data_temp;
        data_temp.len = 0;
        data_temp.data = NULL;
        msg->data = data_temp;
    }
    else
    {
        ProtobufCBinaryData data_temp;
        data_temp.len = data->datasize;
        data_temp.data = malloc(data->datasize);
        memcpy(data_temp.data, data->data, data->datasize);
        msg->data = data_temp;
    }

    data_destroy(data);
}

void put_entry(MessageT *msg, struct table_t *table)
{
    if (msg->keys[0] == NULL || msg->data.len == 0)
    {
        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
        return;
    }

    struct data_t *data = data_create(msg->data.len);
    memcpy(data->data, msg->data.data, msg->data.len);

    if (table_put(table, msg->keys[0], data) == -1)
    {
        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }
    else
    {
        msg->opcode += 1;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }

    data_destroy(data);
}

void get_keys(MessageT *msg, struct table_t *table)
{
    char **keys = table_get_keys(table);

    msg->n_keys = countKeys(keys);
    msg->keys = keys;

    msg->opcode += 1;
    msg->c_type = MESSAGE_T__C_TYPE__CT_KEYS;
}

void table_to_string(MessageT *msg, struct table_t *table)
{
    msg->opcode += 1;
    msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;

    int n_lists = table->n_lists;
    struct list_t **lists = table_print(table);

    msg->n_table = n_lists;
    msg->table = malloc(n_lists * sizeof(MessageT__Table *));

    MessageT__Table *table_temp = malloc(n_lists * sizeof(MessageT__Table));
    for (int i = 0; i < n_lists; i++)
    {
        // Inicia table_temp
        message_t__table__init(&table_temp[i]);

        //Lista das entries
        int n_entries = list_size(lists[i]);
        if (n_entries <= 0)
        {
            table_temp[i].n_entries = 0;
            table_temp[i].entries = NULL;
            msg->table[i] = &table_temp[i];
            continue;
        }

        struct entry_t **entries = list_print(lists[i]);
        table_temp[i].n_entries = n_entries;
        table_temp[i].entries = malloc(n_entries * sizeof(MessageT__Table__Entry *));

        MessageT__Table__Entry *entry_temp = malloc(n_entries * sizeof(MessageT__Table__Entry));
        for (int j = 0; j < n_entries; j++)
        {
            // Inicia entry_temp
            message_t__table__entry__init(&entry_temp[j]);

            // escreve key na entry
            entry_temp[j].key = malloc(strlen(entries[j]->key) + 1);
            strcpy(entry_temp[j].key, entries[j]->key);

            // inicia dados na entry
            ProtobufCBinaryData data_temp;
            data_temp.len = entries[j]->value->datasize;
            data_temp.data = malloc(entries[j]->value->datasize);
            memcpy(data_temp.data, entries[j]->value->data, entries[j]->value->datasize);
            entry_temp[j].data = data_temp;

            table_temp[i].entries[j] = &entry_temp[j];
        }
        msg->table[i] = &table_temp[i];
    }
}