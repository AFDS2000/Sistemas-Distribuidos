/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _TABLE_SKEL_PRIVATE_H
#define _TABLE_SKEL_PRIVATE_H

#include "sdmessage.pb-c.h"
#include "table.h"

void get_table_size(MessageT *msg, struct table_t *table);

void del_entry(MessageT *msg, struct table_t *table);

#endif