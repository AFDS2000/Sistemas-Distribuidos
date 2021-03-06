/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "entry.h"

/* Função que cria uma entry, reservando a memória necessária e
 * inicializando-a com a string e o bloco de dados passados.
 */
struct entry_t *entry_create(char *key, struct data_t *data)
{
    if (key == NULL || data == NULL)
        return NULL;

    struct entry_t *entry = malloc(sizeof(struct entry_t));

    if (entry == NULL)
        return NULL;

    entry->key = key;
    entry->value = data;
    return entry;
}

/* Função que inicializa os elementos de uma entrada na tabela com o
 * valor NULL.
 */
void entry_initialize(struct entry_t *entry)
{
    if (entry && entry->key)
        entry_create(entry->key, NULL);
}

/* Função que elimina uma entry, libertando a memória por ela ocupada
 */
void entry_destroy(struct entry_t *entry)
{
    if (entry)
    {
        if (entry->value)
            data_destroy(entry->value);
        if (entry->key)
            free(entry->key);
        free(entry);
    }
}

/* Função que duplica uma entry, reservando a memória necessária para a
 * nova estrutura.
 */
struct entry_t *entry_dup(struct entry_t *entry)
{
    if (entry && entry->key && entry->value)
    {
        struct entry_t *entry_dup = entry_create(strdup(entry->key), data_dup(entry->value));

        if (entry_dup == NULL)
            return NULL;
        return entry_dup;
    } else {
        return NULL;
    }
}

/* Função que substitui o conteúdo de uma entrada entry_t.
*  Deve assegurar que destroi o conteúdo antigo da mesma.
*/
void entry_replace(struct entry_t *entry, char *new_key, struct data_t *new_value)
{
    if (entry)
    {
        data_destroy(entry->value);
        free(entry->key);
    }

    entry->value = new_value;
    entry->key = new_key;
}

/* Função que compara duas entradas e retorna a ordem das mesmas.
*  Ordem das entradas é definida pela ordem das suas chaves.
*  A função devolve 0 se forem iguais, -1 se entry1<entry2, e 1 caso contrário.
*/
int entry_compare(struct entry_t *entry1, struct entry_t *entry2)
{
    char *key1 = entry1->key;
    char *key2 = entry2->key;
    int num = strcmp(key1, key2);
    if (num == 0)
        return 0;
    else if (num < 0)
        return -1;
    else
        return 1;
}
