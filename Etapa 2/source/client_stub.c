#include <stdio.h>
#include <stdlib.h>

#include "client_stub-private.h"
#include "client_stub.h"
#include "network_client.h"
#include "sdmessage.pb-c.h"

/* Remote table, que deve conter as informações necessárias para estabelecer a comunicação com o servidor. A definir pelo grupo em client_stub-private.h
 */
struct rtable_t server;

/* Função para estabelecer uma associação entre o cliente e o servidor, 
 * em que address_port é uma string no formato <hostname>:<port>.
 * Retorna NULL em caso de erro.
 */
struct rtable_t *rtable_connect(const char *address_port)
{
    int nbytes;
    char str[MAX_MSG];
    str[MAX_MSG - 1] = '\0';
    strncpy(str, "ola", MAX_MSG - 1); // variaveis antes apenas para testar

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
        printf("Execução do programa: ./<path_to_exe>/table.server <port> <n_lists>\n");

        return NULL;
    }
    if (network_connect(&server) == -1)
    {
        return NULL;
    };

    /*

    if ((nbytes = write(server.socket, str, strlen(str))) != strlen(str))
    {
        perror("Erro ao enviar dados ao servidor");
        close(server.socket);
        return NULL;
    }
    int count;
    if ((nbytes = read(server.socket, &count, sizeof(count))) != sizeof(count))
    {
        perror("Erro ao receber dados do servidor");
        close(server.socket);
        return NULL;
    };
    printf("%d", ntohl(count)); */
    return &server;
}

/* Termina a associação entre o cliente e o servidor, fechando a 
 * ligação com o servidor e libertando toda a memória local.
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtable_disconnect(struct rtable_t *rtable);

/* Função para adicionar um elemento na tabela.
 * Se a key já existe, vai substituir essa entrada pelos novos dados.
 * Devolve 0 (ok, em adição/substituição) ou -1 (problemas).
 */
int rtable_put(struct rtable_t *rtable, struct entry_t *entry);

/* Função para obter um elemento da tabela.
 * Em caso de erro, devolve NULL.
 */
struct data_t *rtable_get(struct rtable_t *rtable, char *key);

/* Função para remover um elemento da tabela. Vai libertar 
 * toda a memoria alocada na respetiva operação rtable_put().
 * Devolve: 0 (ok), -1 (key not found ou problemas).
 */
int rtable_del(struct rtable_t *rtable, char *key);

/* Devolve o número de elementos contidos na tabela.
 */
int rtable_size(struct rtable_t *rtable)
{
    unsigned len;
    MessageT msg;
    uint8_t *buf = NULL;

    message_t__init(&msg);
    msg.opcode = 10;
    msg.c_type = 70;
    msg.data = NULL;
    msg.data_size = 0;

    len = message_t__get_packed_size(&msg);

    buf = malloc(len);
    if (buf == NULL)
    {
        fprintf(stdout, "malloc error\n");
        return -1;
    }
    message_t__pack(&msg, buf);

    //enviar

    int nbytes;
    if ((nbytes = write(rtable->socket, buf, len)) != len)
    {
        perror("Erro ao enviar dados ao servidor");
        close(server.socket);
        return -1;
    }
    char *count = "";
    MessageT *recv_msg;
    uint8_t *recv_buf = NULL;

    if ((nbytes = read(rtable->socket, recv_buf, len)) != len)
    {
        perror("Erro ao receber dados do servidor");
        close(server.socket);
        return -1;
    };
    recv_msg = message_t__unpack(NULL, len, recv_buf);
    //printf("%d", ntohl(count));

    return 1;
}

/* Devolve um array de char* com a cópia de todas as keys da tabela,
 * colocando um último elemento a NULL.
 */
char **rtable_get_keys(struct rtable_t *rtable);

/* Liberta a memória alocada por rtable_get_keys().
 */
void rtable_free_keys(char **keys);

/* Função que imprime o conteúdo da tabela remota para o terminal.
 */
void rtable_print(struct rtable_t *rtable);