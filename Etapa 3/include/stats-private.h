#ifndef _STATS_PRIVATE_H
#define _STATS_PRIVATE_H

struct statistics
{
    int nPuts;
    double timePuts;
    int nGets;
    double timeGets;
    int nDels;
    double timeDels;
    int nSizes;
    double timeSizes;
    int nGetKeys;
    double timeGetKeys;
    int nTable_prints;
    double timeTable_prints;
};

#endif