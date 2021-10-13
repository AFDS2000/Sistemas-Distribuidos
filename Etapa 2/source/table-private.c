/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <string.h>

#include "table-private.h"

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