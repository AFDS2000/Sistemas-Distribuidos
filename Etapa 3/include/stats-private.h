/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _STATS_PRIVATE_H
#define _STATS_PRIVATE_H

struct statistics
{
    int nPuts;
    int nGets;
    int nDels;
    int nSizes;
    int nGetKeys;
    int nTable_prints;
    double avg_per_operation;
};

#endif