/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _TABLE_SKEL_PRIVATE_H
#define _TABLE_SKEL_PRIVATE_H

#include "sdmessage.pb-c.h"
#include "entry.h"
#include "list.h"
#include "table.h"
#include "table-private.h"
#include "stats_server-private.h"

void get_table_size(MessageT *msg, struct table_t *table);

void del_entry(MessageT *msg, struct table_t *table);

void get_entry(MessageT *msg, struct table_t *table);

void put_entry(MessageT *msg, struct table_t *table);

void get_keys(MessageT *msg, struct table_t *table);

void table_to_string(MessageT *msg, struct table_t *table);

void get_stats(MessageT *msg, struct statistics *stats);

void write_error(MessageT *msg);

#endif