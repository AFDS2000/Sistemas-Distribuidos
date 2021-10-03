#include "table-private.h"
#include <stdio.h>
#include <string.h>

int hash(char *key, int n)
{
    int length = strlen(key);
    int result = 0, i = 0;
    
    while (key[i] != '\0')
    {
        result += key[i];
        i++;
    }

    return result % n;
}