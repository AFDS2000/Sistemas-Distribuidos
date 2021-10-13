/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "data.h"

/* Função que cria um novo elemento de dados data_t e reserva a memória
 * necessária, especificada pelo parâmetro size 
 */
struct data_t *data_create(int size)
{
    if (size <= 0)
        return NULL;

    struct data_t *d = malloc(sizeof(struct data_t));
    if (d == NULL)
        return NULL;

    d->datasize = size;
    d->data = malloc(size);
    if (d->data == NULL)
    {
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
        return NULL;

    struct data_t *d = malloc(sizeof(struct data_t));
    if (d == NULL)
        return NULL;

    d->datasize = size;
    d->data = data;
    return d;
}

/* Função que elimina um bloco de dados, apontado pelo parâmetro data,
 * libertando toda a memória por ele ocupada.
 */
void data_destroy(struct data_t *data)
{
    if (data)
    {
        free(data->data);
        free(data);
    }
}

/* Função que duplica uma estrutura data_t, reservando a memória
 * necessária para a nova estrutura.
 */
struct data_t *data_dup(struct data_t *data)
{
    if (data == NULL || data->data == NULL)
        return NULL;

    struct data_t *new_data = data_create(data->datasize);
    if (new_data == NULL)
        return NULL;

    memcpy(new_data->data, data->data, data->datasize);
    return new_data;
}

/* Função que substitui o conteúdo de um elemento de dados data_t.
*  Deve assegurar que destroi o conteúdo antigo do mesmo.
*/
void data_replace(struct data_t *data, int new_size, void *new_data)
{
    data->datasize = new_size;
    free(data->data);
    data->data = new_data;
}