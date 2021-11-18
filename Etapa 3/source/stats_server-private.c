#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "stats_server-private.h"
#include "sdmessage.pb-c.h"

static struct statistics *stats_server;

/* Inicia a struct stats com todos os parametros a 0
 */
void stats_server_init()
{
    stats_server = malloc(sizeof(struct statistics));
    stats_server->nPuts = 0;
    stats_server->timePuts = 0;
    stats_server->nGets = 0;
    stats_server->timeGets = 0;
    stats_server->nDels = 0;
    stats_server->timeDels = 0;
    stats_server->nSizes = 0;
    stats_server->timeSizes = 0;
    stats_server->nGetKeys = 0;
    stats_server->timeGetKeys = 0;
    stats_server->nTable_prints = 0;
    stats_server->timeTable_prints = 0;
}

void stats_server_destroy()
{
    free(stats_server);
}

/* Atualiza as estatisticas da struct stats
*/
void update_stats(int opcode, double time)
{
    switch (opcode)
    {
    case MESSAGE_T__OPCODE__OP_SIZE:
        stats_server->timeSizes = stats_server->timeSizes * stats_server->nSizes + time;
        stats_server->nSizes++;
        stats_server->timeSizes /= stats_server->nSizes;
        printf("Size = %d, %f", stats_server->nSizes, stats_server->timeSizes);
        break;
    case MESSAGE_T__OPCODE__OP_DEL:
        stats_server->timeDels = stats_server->timeDels * stats_server->nDels + time;
        stats_server->nDels++;
        stats_server->timeDels /= stats_server->nDels;
        break;
    case MESSAGE_T__OPCODE__OP_GET:
        stats_server->timeGets = stats_server->timeGets * stats_server->nGets + time;
        stats_server->nGets++;
        stats_server->timeGets /= stats_server->nGets;
        break;
    case MESSAGE_T__OPCODE__OP_PUT:
        stats_server->timePuts = stats_server->timePuts * stats_server->nPuts + time;
        stats_server->nPuts++;
        stats_server->timePuts /= stats_server->nPuts;
        break;
    case MESSAGE_T__OPCODE__OP_GETKEYS:
        stats_server->timeGetKeys = stats_server->timeGetKeys * stats_server->nGetKeys + time;
        stats_server->nGetKeys++;
        stats_server->timeGetKeys /= stats_server->nGetKeys;
        break;
    case MESSAGE_T__OPCODE__OP_PRINT:
        stats_server->timeTable_prints = stats_server->timeTable_prints * stats_server->nTable_prints + time;
        stats_server->nTable_prints++;
        stats_server->timeTable_prints /= stats_server->nTable_prints;
        break;
    default:
        return;
    }
}

struct statistics *getStats()
{
    return stats_server;
}