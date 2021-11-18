/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _STATS_SERVER_PRIVATE_H
#define _STATS_SERVER_PRIVATE_H

#include "stats-private.h"
#include "stats_server-private.h"

void stats_server_init();

void stats_server_destroy();

void update_stats(int opcode, double time);
#endif
