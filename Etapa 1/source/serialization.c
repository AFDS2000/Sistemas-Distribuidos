#include "serialization.h"
#include "data.h"
#include "entry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Serializa uma estrutura data num buffer que será alocado
 * dentro da função. Além disso, retorna o tamanho do buffer
 * alocado ou -1 em caso de erro.
 */
int data_to_buffer(struct data_t *data, char **data_buf)
{
    if (data == NULL || data_buf == NULL)
        return -1;
    
    char *buff = malloc(sizeof(int) + data->datasize);

    memcpy(buff, &data->datasize, sizeof(int));
    memcpy(buff + sizeof(int), data->data, data->datasize);

    *data_buf = buff;
    return sizeof(int) + data->datasize;
}

/* De-serializa a mensagem contida em data_buf, com tamanho
 * data_buf_size, colocando-a e retornando-a numa struct
 * data_t, cujo espaco em memoria deve ser reservado.
 * Devolve NULL em caso de erro.
 */
struct data_t *buffer_to_data(char *data_buf, int data_buf_size)
{
    if (data_buf_size <= 0 || data_buf == NULL)
        return NULL;

    int tamanho = 0;
    memcpy(&tamanho, data_buf, sizeof(int));
    struct data_t *data = data_create(tamanho);

    memcpy(data->data, data_buf + sizeof(int), tamanho); // copiar data de forma a ter um objeto diferente para evitar conflitos nos frees
    return data;
}

/* Serializa uma estrutura entry num buffer que sera alocado
 * dentro da função. Além disso, retorna o tamanho deste
 * buffer alocado ou -1 em caso de erro.
 */
int entry_to_buffer(struct entry_t *data, char **entry_buf)
{
    if (data == NULL || entry_buf == NULL)
        return -1;

    int size = sizeof(int) + strlen(data->key) + 1 + sizeof(int) + data->value->datasize;
    int size_key = strlen(data->key) + 1;
    char *buff = malloc(size);
    
    //converter data para serial
    char *data_buf;
    int data_buf_size = data_to_buffer(data->value, &data_buf);

    memcpy(buff, &size_key, sizeof(int));                           // tamanho da key
    memcpy(buff + sizeof(int), data->key, size_key);                // key
    memcpy(buff + sizeof(int) + size_key, data_buf, data_buf_size); // data buffer

    free(data_buf);

    *entry_buf = buff;
    return size;
}

/* De-serializa a mensagem contida em entry_buf, com tamanho
 * entry_buf_size, colocando-a e retornando-a numa struct
 * entry_t, cujo espaco em memoria deve ser reservado.
 * Devolve NULL em caso de erro.
 */
struct entry_t *buffer_to_entry(char *entry_buf, int entry_buf_size)
{
    if (entry_buf_size <= 0 || entry_buf == NULL)
        return NULL;
    
    int key_size = 0;
    memcpy(&key_size, entry_buf, sizeof(int));

    char *key = malloc(key_size);
    memcpy(key, entry_buf + sizeof(int), key_size);

    int data_buffer_size = 0;
    memcpy(&data_buffer_size, entry_buf + sizeof(int) + key_size, sizeof(int));

    struct data_t *data = buffer_to_data(entry_buf + (sizeof(int) + key_size), data_buffer_size);
    struct entry_t *entry = entry_create(key, data);
    return entry;
}
