/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "table_skel-private.h"

/**
 * Função conta número de chaves que existe na lista de chaves
 * 
 * @param keys lista de chaves
 * @return int número de chaves da lista de chaves
 */
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
    msg->table = table_print(table);
}

void get_stats(MessageT *msg, struct statistics *stats)
{
    msg->opcode += 1;
    msg->c_type = MESSAGE_T__C_TYPE__CT_RESULT;
    msg->stats = malloc(sizeof(MessageT__StatsT));
    message_t__stats_t__init(msg->stats);

    msg->stats->nputs = stats->nPuts;
    msg->stats->timeputs = stats->timePuts;
    msg->stats->ngets = stats->nGets;
    msg->stats->timegets = stats->timeGets;
    msg->stats->ndels = stats->nDels;
    msg->stats->timedels = stats->timeDels;
    msg->stats->nsizes = stats->nSizes;
    msg->stats->timesizes = stats->timeSizes;
    msg->stats->ngetkeys = stats->nGetKeys;
    msg->stats->timegetkeys = stats->timeGetKeys;
    msg->stats->ntable_prints = stats->nTable_prints;
    msg->stats->timetable_prints = stats->timeTable_prints;

}