#include <stdio.h>
#include <stdlib.h>

#include "sdmessage.pb-c.h"
#include "message-private.h"
#include "inet-private.h"
#include "table_skel.h"
#include "network_server.h"

static struct sockaddr_in server;

/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar descritor do socket (OK) ou -1 (erro).
 */
int network_server_init(short port)
{
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htons(INADDR_ANY);

    // bind
    if (bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        close(sockfd);
        return -1;
    }

    // listen
    if (listen(sockfd, 0) < 0)
    {
        close(sockfd);
        return -1;
    }

    return sockfd;
}

/* Esta função deve:
 * - Aceitar uma conexão de um cliente;
 * - Receber uma mensagem usando a função network_receive;
 * - Entregar a mensagem de-serializada ao skeleton para ser processada;
 * - Esperar a resposta do skeleton;
 * - Enviar a resposta ao cliente usando a função network_send.
 */
int network_main_loop(int listening_socket)
{
    int connsockfd;
    // int nbytes, count;
    int client;
    socklen_t size_client;

    printf("Waiting conection\n");

    // accept bloqueia à espera de pedidos de conexão.
    // Quando retorna já foi feito o "three-way handshake" e connsockfd é uma
    // socket pronta a comunicar com o cliente.
    while ((connsockfd = accept(listening_socket, (struct sockaddr *)&client, &size_client)) != -1)
    {

        printf("Conection accept\n");
        //primeiro read é para receber o size
        int buffer_len_recv;
        int d = read(connsockfd, &buffer_len_recv, sizeof(int)); // recebe o unsigned
        int normal = ntohl(buffer_len_recv);

        MessageT *recv_msg;
        uint8_t *recv_buf = malloc(normal);

        int b = read_all(connsockfd, recv_buf, normal);
        recv_msg = message_t__unpack(NULL, normal, recv_buf); // ler msg

        // envio

        unsigned len;
        MessageT msg;
        uint8_t *buf = NULL;

        message_t__init(&msg);
        msg.opcode = 10;
        msg.c_type = 70;
        msg.data = NULL; //"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890";
        msg.data_size = 0;

        len = message_t__get_packed_size(&msg);

        buf = malloc(len);
        if (buf == NULL)
        {
            fprintf(stdout, "malloc error\n");
            return -1;
        }
        message_t__pack(&msg, buf);

        int buffer_len = htonl(len);
        int unsig = write(connsockfd, &buffer_len, sizeof(buffer_len)); //enviar os len

        int a = write_all(connsockfd, buf, len); // enviar msg

        //segundo write é para enviar a msg

        // Fecha socket referente a esta conexão
        close(connsockfd);
    }
    // Fecha socket
    close(listening_socket);
    return 0;
}

/* Esta função deve:
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura MessageT.
 */
MessageT *network_receive(int client_socket)
{
    MessageT *recv_msg = NULL;
    int bufsize;
    uint8_t *buf = malloc(sizeof(uint8_t));

    if ((bufsize = read_all(client_socket, buf, sizeof(uint8_t))) < 0)
    {
        return NULL;
    }
    return NULL;
}

/* Esta função deve:
 * - Serializar a mensagem de resposta contida em msg;
 * - Libertar a memória ocupada por esta mensagem;
 * - Enviar a mensagem serializada, através do client_socket.
 */
int network_send(int client_socket, MessageT *msg)
{
    return 0;
}

/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close()
{

    return 0;
}