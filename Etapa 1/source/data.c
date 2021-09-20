#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Função que cria um novo elemento de dados data_t e reserva a memória
 * necessária, especificada pelo parâmetro size 
 */
struct data_t *data_create(int size)
{
    if (size <= 0)
    {
        return NULL;
    }
    struct data_t *d = (struct data_t *)malloc(sizeof(struct data_t));
    if (d == NULL)
    {
        free(d);
        return NULL;
    }
    d->datasize = size;
    d->data = malloc(size);
    if (d->data == NULL)
    {
        free(d->data);
        free(d);
        return NULL;
    }
    return d;
}
/* Função idêntica à anterior, mas que inicializa os dados de acordo com
 * o parâmetro data.
 */
struct data_t *data_create2(int size, void *data)
{
    if (data == NULL || size <= 0)
    {
        return NULL;
    }
    struct data_t *d = (struct data_t *)malloc(sizeof(struct data_t));
    if (d == NULL)
    {
        free(d);
        return NULL;
    }
    d->datasize = size;
    d->data = data;
    if (d->data == NULL)
    {
        free(d->data);
        free(d);
        return NULL;
    }
    return d;
}

/* Função que elimina um bloco de dados, apontado pelo parâmetro data,
 * libertando toda a memória por ele ocupada.
 */
void data_destroy(struct data_t *data)
{
    if (data != NULL)
    {
        free(data->data);
        free(data);
    }
    else
    {
        free(data);
    }
}

/* Função que duplica uma estrutura data_t, reservando a memória
 * necessária para a nova estrutura.
 */
struct data_t *data_dup(struct data_t *data)
{
    if (data == NULL || data->data == NULL)
    {
        return NULL;
    }
    struct data_t *d = data_create(data->datasize);
    if (d == NULL)
    {
        free(d);
        return NULL;
    }
    memcpy(d->data, data->data, data->datasize);
    return d;
}

/* Função que substitui o conteúdo de um elemento de dados data_t.
*  Deve assegurar que destroi o conteúdo antigo do mesmo.
*/
void data_replace(struct data_t *data, int new_size, void *new_data)
{
}
