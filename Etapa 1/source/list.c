#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "list-private.h"

/* Função que cria uma nova lista (estrutura list_t a ser definida pelo
 * grupo no ficheiro list-private.h).
 * Em caso de erro, retorna NULL.
 */
struct list_t *list_create()
{
    struct list_t *list = malloc(sizeof(struct list_t));
    if (list == NULL)
    {
        free(list);
        return NULL;
    }
    list->head = NULL;

    list->length = 0;
    return list;
}

/* Função que elimina uma lista, libertando *toda* a memoria utilizada
 * pela lista.
 */
void list_destroy(struct list_t *list)
{
    while (list->head)
    {
        struct node_t *next = list->head->next;
        if (list->head->value != NULL)
        {
            entry_destroy(list->head->value);
            free(list->head);
        }
        list->head = next;
    }
    free(list->head);
    free(list);
}

/* Função que adiciona no final da lista (tail) a entry passada como
* argumento caso não exista na lista uma entry com key igual àquela
* que queremos inserir.
* Caso exista, os dados da entry (value) já existente na lista serão
* substituídos pelos os da nova entry.
* Retorna 0 (OK) ou -1 (erro).
*/

int list_add(struct list_t *list, struct entry_t *entry)
{
    if (list == NULL)
        return -1;

    struct node_t *new_node = malloc(sizeof(struct node_t));
    if (new_node == NULL)
    {
        free(new_node);
        return -1;
    }
    new_node->next = NULL;
    new_node->value = entry;

    //Se a lista tiver vazia
    if (list->length == 0)
    {
        list->head = new_node;
        list->length++;
        return 0;
    }

    struct node_t *node = list->head;
    while (node)
    {
        if (entry_compare(entry, node->value) == 0)
        {
            entry_destroy(node->value);
            node->value = entry;
            free(new_node);
            return 0;
        }

        else if (node->next == NULL)
        {
            node->next = new_node;
            list->length++;
            return 0;
        }
        else
            node = node->next;
    }

    return 0;
}

/* Função que elimina da lista a entry com a chave key.
 * Retorna 0 (OK) ou -1 (erro).
 */
int list_remove(struct list_t *list, char *key)
{
    if (list == NULL)
        return -1;

    //Se a lista tiver vazia
    if (list->head == NULL)
        return 0;

    struct node_t *node = list->head;

    //Se a head for a key que se procura
    if (strcmp(key, node->value->key) == 0)
    {
        entry_destroy(node->value);
        list->head = node->next;
        free(node);
        list->length--;
        return 0;
    }

    struct node_t *node_next = node->next;
    while (node_next)
    {
        if (strcmp(key, node_next->value->key) == 0)
        {
            entry_destroy(node_next->value);
            node->next = node_next->next;
            free(node_next);
            list->length--;
            return 0;
        }
        else
        {
            node = node_next;
            node_next = node_next->next;
        }
    }

    return 0;
}

/* Função que obtém da lista a entry com a chave key.
 * Retorna a referência da entry na lista ou NULL em caso de erro.
 * Obs: as funções list_remove e list_destroy vão libertar a memória
 * ocupada pela entry ou lista, significando que é retornado NULL
 * quando é pretendido o acesso a uma entry inexistente.
*/
struct entry_t *list_get(struct list_t *list, char *key)
{
    if (list == NULL)
        return NULL;

    struct node_t *node = list->head;
    while (node)
    {
        if (strcmp(node->value->key, key) == 0)
            return node->value;
        if (node->next == NULL)
            return NULL;
        node = node->next;
    }

    return NULL;
}

/* Função que retorna o tamanho (número de elementos (entries)) da lista,
 * ou -1 (erro).
 */
int list_size(struct list_t *list)
{
    return list->length;
}

/* Função que devolve um array de char* com a cópia de todas as keys da 
 * tabela, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
char **list_get_keys(struct list_t *list)
{
    char **lista = malloc(sizeof(char *) * (list->length + 1));

    struct node_t *node = list->head;
    int i = 0;
    while (node)
    {
        lista[i] = malloc(sizeof(char) * (strlen(node->value->key) + 1));
        memcpy(lista[i], node->value->key, (strlen(node->value->key) + 1));
        node = node->next;
        i++;
    }

    return lista;
}

/* Função que liberta a memória ocupada pelo array das keys da tabela,
 * obtido pela função list_get_keys.
 */
void list_free_keys(char **keys)
{
    int i = 0;
    while (keys[i])
    {
        free(keys[i]);
        i++;
    }
    free(keys);
    return;
}

/* Função que imprime o conteúdo da lista para o terminal.
 */
void list_print(struct list_t *list)
{
    struct node_t *node = list->head;
    while (node)
    {
        char *key = node->value->key;
        struct data_t *data = node->value->value;
        printf("\n-----------------------------------------------\nkey: %s \ndatasize: %d \ndata: %s", key, data->datasize, (char *)data->data);
        node = node->next;
    }
    printf("\n-----------------------------------------------\n");
}