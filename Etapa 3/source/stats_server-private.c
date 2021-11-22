#include <stdio.h>
#include <stdlib.h>

#include "stats_server-private.h"
#include "sdmessage.pb-c.h"

static struct statistics *stats_server;

/* Inicia a struct stats com todos os parametros a 0
 */
void stats_server_init()
{
    stats_server = malloc(sizeof(struct statistics));
    stats_server->nPuts = 0;
    stats_server->nGets = 0;
    stats_server->nDels = 0;
    stats_server->nSizes = 0;
    stats_server->nGetKeys = 0;
    stats_server->nTable_prints = 0;
    stats_server->avg_per_operation = 0;
}

void stats_server_destroy()
{
    free(stats_server);
}

/* Atualiza as estatisticas da struct stats
*/
void update_stats(int opcode, double time)
{
    int total = stats_server->nSizes + stats_server->nDels + stats_server->nGets +
        stats_server->nPuts + stats_server->nGetKeys + stats_server->nTable_prints;
    stats_server->avg_per_operation *= total;
    stats_server->avg_per_operation += time;

    switch (opcode)
    {
    case MESSAGE_T__OPCODE__OP_SIZE:
        stats_server->nSizes++;
        break;
    case MESSAGE_T__OPCODE__OP_DEL:
        stats_server->nDels++;
        break;
    case MESSAGE_T__OPCODE__OP_GET:
        stats_server->nGets++;
        break;
    case MESSAGE_T__OPCODE__OP_PUT:
        stats_server->nPuts++;
        break;
    case MESSAGE_T__OPCODE__OP_GETKEYS:
        stats_server->nGetKeys++;
        break;
    case MESSAGE_T__OPCODE__OP_PRINT:
        stats_server->nTable_prints++;
        break;
    default:
        return;
    }
    
    total++;
    stats_server->avg_per_operation /= total;
}

struct statistics *getStats()
{
    return stats_server;
}