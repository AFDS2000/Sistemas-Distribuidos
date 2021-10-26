#include <stdio.h>
#include <stdlib.h>

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
        fprintf(stdout, "malloc error\n");
        return NULL;
    }
    unsigned int buffer_len = htonl(len);
    if (write(rtable->socket, &buffer_len, sizeof(buffer_len)) < 0)
    {
        perror("Erro ao enviar dados para o servidor");
        return NULL;
    }; // enviar os len
    message_t__pack(msg, buf);
    if (write_all(rtable->socket, buf, len) < 0)
    {
        perror("Erro ao enviar dados para o servidor");
        return NULL;
    }; // enviar msg
    //message_t__free_unpacked(msg, NULL);
    free(buf);

    unsigned int buffer_len_recv;
    if (read(rtable->socket, &buffer_len_recv, sizeof(buffer_len_recv)) < 0)
    {
        perror("Erro ao receber dados do servidor");
        return NULL;
    }; // ler o len
    unsigned int len_recv = ntohl(buffer_len_recv);

    MessageT *recv_msg;
    uint8_t *recv_buf = malloc(len_recv);
    if (recv_buf == NULL)
    {
        fprintf(stdout, "malloc error\n");
        return NULL;
    }

    if (read_all(rtable->socket, recv_buf, len_recv) < 0)
    {
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
int network_close(struct rtable_t *rtable);