/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "network_client.h"
#include "client_stub-private.h"
#include "message-private.h"

/* Esta função deve:
 * - Obter o endereço do servidor (struct sockaddr_in) a base da
 *   informação guardada na estrutura rtable;
 * - Estabelecer a ligação com o servidor;
 * - Guardar toda a informação necessária (e.g., descritor do socket)
 *   na estrutura rtable;
 * - Retornar 0 (OK) ou -1 (erro).
 */
int network_connect(struct rtable_t *rtable)
{
    signal(SIGPIPE, SIG_IGN);

    if ((rtable->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Erro ao criar socket TCP");
        return -1;
    }
    rtable->addr.sin_family = AF_INET;           // família de endereços
    rtable->addr.sin_port = htons(rtable->port); // Porta TCP
    if (inet_pton(AF_INET, rtable->ip, &rtable->addr.sin_addr) < 1)
    {
        printf("Erro ao converter IP\n");
        close(rtable->socket);
        return -1;
    }
    if (connect(rtable->socket, (struct sockaddr *)&rtable->addr, sizeof(rtable->addr)) < 0)
    {
        perror("Erro ao conectar-se ao servidor");
        close(rtable->socket);
        return -1;
    }
    return 0;
}

/* Esta função deve:
 * - Obter o descritor da ligação (socket) da estrutura rtable_t;
 * - Reservar memória para serializar a mensagem contida em msg;
 * - Serializar a mensagem contida em msg;
 * - Enviar a mensagem serializada para o servidor;
 * - Libertar a memória ocupada pela mensagem serializada enviada;
 * - Esperar a resposta do servidor;
 * - Reservar a memória para a mensagem serializada recebida;
 * - De-serializar a mensagem de resposta, reservando a memória 
 *   necessária para a estrutura MessageT que é devolvida;
 * - Libertar a memória ocupada pela mensagem serializada recebida;
 * - Retornar a mensagem de-serializada ou NULL em caso de erro.
 */
MessageT *network_send_receive(struct rtable_t *rtable, MessageT *msg)
{
    unsigned int len;
    uint8_t *buf = NULL;
    len = message_t__get_packed_size(msg);
    buf = malloc(len);
    if (buf == NULL)
    {
        perror("malloc error\n");
        return NULL;
    }

    // enviar o len da msg
    unsigned int buffer_len = htonl(len);
    uint8_t *buffer_recv_buf = malloc(sizeof(unsigned int));
    memcpy(buffer_recv_buf, &buffer_len, sizeof(unsigned int));

    if (write_all(rtable->socket, buffer_recv_buf, sizeof(buffer_len)) <= 0)
    {
        free(buf);
        perror("Erro ao enviar dados para o servidor");
        return NULL;
    };
    free(buffer_recv_buf);

    // enviar msg
    message_t__pack(msg, buf);
    if (write_all(rtable->socket, buf, len) < 0)
    {
        free(buf);

        perror("Erro ao enviar dados para o servidor");
        return NULL;
    }
    free(buf);

    // receber o len da msg
    uint8_t *len_recv_buf = malloc(sizeof(unsigned int));

    if (read_all(rtable->socket, len_recv_buf, sizeof(unsigned int)) <= 0)
    {
        free(len_recv_buf);
        perror("Erro ao receber dados do servidor");
        return NULL;
    };

    unsigned int len_recv;
    memcpy(&len_recv, len_recv_buf, sizeof(unsigned int));
    free(len_recv_buf);

    len_recv = ntohl(len_recv);
    MessageT *recv_msg;
    uint8_t *recv_buf = malloc(len_recv);
    if (recv_buf == NULL)
    {
        fprintf(stdout, "malloc error\n");
        return NULL;
    }

    // receber a msg
    if (read_all(rtable->socket, recv_buf, len_recv) <= 0)
    {
        free(recv_buf);
        perror("Erro ao receber dados do servidor");
        return NULL;
    };
    recv_msg = message_t__unpack(NULL, len_recv, recv_buf); // ler msg

    free(recv_buf);
    return recv_msg;
}

/* A função network_close() fecha a ligação estabelecida por
 * network_connect().
 */
int network_close(struct rtable_t *rtable)
{
    return close(rtable->socket);
}