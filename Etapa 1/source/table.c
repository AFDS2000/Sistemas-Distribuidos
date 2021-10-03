#include "table.h"
#include "table-private.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Função para criar/inicializar uma nova tabela hash, com n
 * linhas (n = módulo da função hash)
 * Em caso de erro retorna NULL.
 */
struct table_t *table_create(int n)
{
    struct table_t *table = malloc(sizeof(struct table_t));
    table->size = n;
    table->count = 0;
    table->items = (struct list_t **)calloc(table->size, sizeof(struct list_t *));
    for (int i = 0; i < table->size; i++)
        table->items[i] = NULL;

    return table;
}

/* Função para libertar toda a memória ocupada por uma tabela.
 */
void table_destroy(struct table_t *table)
{
    if (table == NULL)
        return;
    for (int i = 0; i < table->size; i++)
    {
        struct list_t *item = table->items[i];
        if (item != NULL)
            list_destroy(item);
    }

    free(table->items);
    free(table);
}

/* Função para adicionar um par chave-valor à tabela.
 * Os dados de entrada desta função deverão ser copiados, ou seja, a
 * função vai *COPIAR* a key (string) e os dados para um novo espaço de
 * memória que tem de ser reservado. Se a key já existir na tabela,
 * a função tem de substituir a entrada existente pela nova, fazendo
 * a necessária gestão da memória para armazenar os novos dados.
 * Retorna 0 (ok) ou -1 em caso de erro.
 */
int table_put(struct table_t *table, char *key, struct data_t *value)
{
    //Cópia dos dados
    char *key_copy = malloc(strlen(key) + 1);
    strcpy(key_copy, key);

    struct data_t *data = data_dup(value);
    if (data == NULL)
        return -1;

    struct entry_t *entry = entry_create(key_copy, data);
    if (entry == NULL)
        return -1;

    //Adicionar na hash table
    int i = hash(key, table->size);
    struct entry_t *entry2 = list_get(table->items[i], key_copy);

    if (table->items[i] == NULL)
        table->items[i] = list_create();

    int error = list_add(table->items[i], entry);
    if (error == -1)
        return error;

    //Verificar se já existia na lista
    if (entry2 == NULL)
        table->count++;
    return 0;
}

/* Função para obter da tabela o valor associado à chave key.
 * A função deve devolver uma cópia dos dados que terão de ser
 * libertados no contexto da função que chamou table_get, ou seja, a
 * função aloca memória para armazenar uma *CÓPIA* dos dados da tabela,
 * retorna o endereço desta memória com a cópia dos dados, assumindo-se
 * que esta memória será depois libertada pelo programa que chamou
 * a função.
 * Devolve NULL em caso de erro.
 */
struct data_t *table_get(struct table_t *table, char *key)
{
    int i = hash(key, table->size);
    struct entry_t *entry = list_get(table->items[i], key);
    if (entry == NULL)
        return NULL;

    return data_dup(entry->value);
}

/* Função para remover um elemento da tabela, indicado pela chave key, 
 * libertando toda a memória alocada na respetiva operação table_put.
 * Retorna 0 (ok) ou -1 (key not found).
 */
int table_del(struct table_t *table, char *key)
{
    int i = hash(key, table->size);
    int error = list_remove(table->items[i], key);

    if (error == -1)
        return error;

    table->count--;
    return 0;
}

/* Função que devolve o número de elementos contidos na tabela.
 */
int table_size(struct table_t *table)
{
    return table->count;
}

/* Função que devolve um array de char* com a cópia de todas as keys da
 * tabela, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
char **table_get_keys(struct table_t *table)
{
    int counter = 0;
    char **tableKeys = malloc(sizeof(char *) * (table->count + 1));

    for (int i = 0; i < table->size; i++)
    {
        int size = list_size(table->items[i]);
        char **listKeys = list_get_keys(table->items[i]);
        for (int j = 0; j < size; j++)
        {
            tableKeys[counter] = malloc(sizeof(char) * (strlen(listKeys[j]) + 1));
            memcpy(tableKeys[counter], listKeys[j], (strlen(listKeys[j]) + 1));
            counter++;
        }
        list_free_keys(listKeys);
    }
    
    tableKeys[counter] = NULL;
    return tableKeys;
}

/* Função que liberta toda a memória alocada por table_get_keys().
 */
void table_free_keys(char **keys)
{
    int i = 0;
    while (keys[i] != NULL)
    {
        free(keys[i]);
        i++;
    }
    free(keys);
    return;
}

/* Função que imprime o conteúdo da tabela.
 */
void table_print(struct table_t *table)
{
    return;
}