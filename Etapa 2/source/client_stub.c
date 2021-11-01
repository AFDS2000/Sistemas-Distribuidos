#include <stdio.h>
#include <stdlib.h>

#include "table.h"
#include "client_stub-private.h"
#include "client_stub.h"
#include "network_client.h"
#include "sdmessage.pb-c.h"
#include "message-private.h"

/* Remote table, que deve conter as informações necessárias para estabelecer a comunicação com o servidor. A definir pelo grupo em client_stub-private.h
 */
struct rtable_t server;

/* Função para estabelecer uma associação entre o cliente e o servidor,
 * em que address_port é uma string no formato <hostname>:<port>.
 * Retorna NULL em caso de erro.
 */
struct rtable_t *rtable_connect(const char *address_port)
{
    char *addr_port = strdup(address_port);
    char *token;
    const char s[2] = ":";
    token = strtok(addr_port, s);
    server.ip = token;
    token = strtok(NULL, s);

    server.port = atoi(token);
    token = strtok(NULL, s);
    if (token)
    {
        printf("Erro na execução\n");
        printf("Uso: ./<path_to_exe>/table-client <address>:<port>\n");
        printf("Exemplo de uso: ./binary/table-client 127.0.0.1:12345\n");

        return NULL;
    }
    if (network_connect(&server) == -1)
    {
        return NULL;
    };
    free(addr_port);
    return &server;
}

/* Termina a associação entre o cliente e o servidor, fechando a
 * ligação com o servidor e libertando toda a memória local.
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtable_disconnect(struct rtable_t *rtable)
{
    return network_close(rtable);
}

/* Função para adicionar um elemento na tabela.
 * Se a key já existe, vai substituir essa entrada pelos novos dados.
 * Devolve 0 (ok, em adição/substituição) ou -1 (problemas).
 */
int rtable_put(struct rtable_t *rtable, struct entry_t *entry)
{
    MessageT msg;

    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg.c_type = MESSAGE_T__C_TYPE__CT_ENTRY;

    msg.n_keys = 1;
    msg.keys = malloc(sizeof(char *));
    msg.keys[0] = entry->key;

    ProtobufCBinaryData data_temp;
    data_temp.len = entry->value->datasize;
    data_temp.data = malloc(entry->value->datasize);
    memcpy(data_temp.data, entry->value->data, entry->value->datasize);
    msg.data = data_temp;

    MessageT *msg_recv = network_send_receive(rtable, &msg);
    free(msg.keys);
    free(data_temp.data);
    if (msg_recv->opcode != msg.opcode + 1)
    {

        message_t__free_unpacked(msg_recv, NULL);
        return -1;
    }
    message_t__free_unpacked(msg_recv, NULL);

    return 0;
}

/* Função para obter um elemento da tabela.
 * Em caso de erro, devolve NULL.
 */
struct data_t *rtable_get(struct rtable_t *rtable, char *key)
{
    MessageT msg;

    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_GET;
    msg.c_type = MESSAGE_T__C_TYPE__CT_KEY;

    msg.n_keys = 1;
    msg.keys = malloc(sizeof(char *));
    msg.keys[0] = key;

    MessageT *msg_recv = network_send_receive(rtable, &msg);

    free(msg.keys);
    if (msg_recv->opcode != msg.opcode + 1 || msg_recv->data.len == 0)
    {
        message_t__free_unpacked(msg_recv, NULL);
        return NULL;
    }

    struct data_t *data = data_create(msg_recv->data.len);
    memcpy(data->data, msg_recv->data.data, data->datasize);

    message_t__free_unpacked(msg_recv, NULL);

    return data;
}

/* Função para remover um elemento da tabela. Vai libertar
 * toda a memoria alocada na respetiva operação rtable_put().
 * Devolve: 0 (ok), -1 (key not found ou problemas).
 */
int rtable_del(struct rtable_t *rtable, char *key)
{
    MessageT msg;

    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_DEL;
    msg.c_type = MESSAGE_T__C_TYPE__CT_KEY;

    msg.n_keys = 1;
    msg.keys = malloc(sizeof(char *));
    msg.keys[0] = key;

    MessageT *msg_recv = network_send_receive(rtable, &msg);
    free(msg.keys);
    if (msg_recv->opcode != msg.opcode + 1)
    {
        message_t__free_unpacked(msg_recv, NULL);
        return -1;
    }

    message_t__free_unpacked(msg_recv, NULL);
    return 0;
}

/* Devolve o número de elementos contidos na tabela.
 */
int rtable_size(struct rtable_t *rtable)
{
    MessageT msg;

    message_t__init(&msg);
    msg.opcode = MESSAGE_T__OPCODE__OP_SIZE;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;

    MessageT *msg_recv = network_send_receive(rtable, &msg);
    if (!msg_recv)
    {
        message_t__free_unpacked(msg_recv, NULL);

        close(rtable->socket);
        return -1;
    }
    if (msg_recv->opcode != msg.opcode + 1)
    {
        message_t__free_unpacked(msg_recv, NULL);
        return -1;
    }

    int size = msg_recv->table_size;
    message_t__free_unpacked(msg_recv, NULL);

    return size;
}

/* Devolve um array de char* com a cópia de todas as keys da tabela,
 * colocando um último elemento a NULL.
 */
char **rtable_get_keys(struct rtable_t *rtable)
{
    MessageT msg;

    message_t__init(&msg);

    msg.opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;

    MessageT *msg_recv = network_send_receive(rtable, &msg);
    if (msg_recv->opcode != msg.opcode + 1)
    {
        message_t__free_unpacked(msg_recv, NULL);
        return NULL;
    }

    char **lista = malloc(sizeof(char *) * (msg_recv->n_keys + 1));

    int i = 0;
    for (i = 0; i < msg_recv->n_keys; i++)
    {
        int size_key = strlen(msg_recv->keys[i]) + 1;
        lista[i] = malloc(size_key);
        memcpy(lista[i], msg_recv->keys[i], size_key);
    }
    lista[i] = NULL;
    message_t__free_unpacked(msg_recv, NULL);

    return lista;
}

/* Liberta a memória alocada por rtable_get_keys().
 */
void rtable_free_keys(char **keys)
{
    table_free_keys(keys);
    return;
}

/* Função que imprime o conteúdo da tabela remota para o terminal.
 */
void rtable_print(struct rtable_t *rtable)
{

    MessageT msg;

    message_t__init(&msg);

    msg.opcode = MESSAGE_T__OPCODE__OP_PRINT;
    msg.c_type = MESSAGE_T__C_TYPE__CT_NONE;

    MessageT *msg_recv = network_send_receive(rtable, &msg);
    if (msg_recv->opcode != msg.opcode + 1 && msg_recv->c_type != MESSAGE_T__C_TYPE__CT_TABLE)
    {
        message_t__free_unpacked(msg_recv, NULL);
    }

    for (int i = 0; i < msg_recv->n_table; i++)
    {
        for (int j = 0; j < msg_recv->table[i]->n_entries; j++)
        {
            printf("\n-----------------------\nkey: %s \ndata: ", msg_recv->table[i]->entries[j]->key);
        
            for (int k = 0; k < msg_recv->table[i]->entries[j]->data.len; k++)
            {
                printf("%c", msg_recv->table[i]->entries[j]->data.data[k]);
            }
            printf("\n-----------------------\n");
        }
    }
    message_t__free_unpacked(msg_recv, NULL);
}