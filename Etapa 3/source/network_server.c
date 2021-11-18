/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
//#include <pthread.h>

#include "table_skel.h"
#include "network_server.h"
#include "inet-private.h"
#include "message-private.h"
#include "table.h"

/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar descritor do socket (OK) ou -1 (erro).
 */
int network_server_init(short port)
{
    signal(SIGPIPE, SIG_IGN);
    int sockfd;
    struct sockaddr_in server;

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return -1;

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htons(INADDR_ANY);

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {
        close(sockfd);
        return -1;
    }

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
    struct sockaddr_in client = {0};
    socklen_t size_client = 0;

    // accept bloqueia à espera de pedidos de conexão.
    // Quando retorna já foi feito o "three-way handshake" e connsockfd é uma
    // socket pronta a comunicar com o cliente.
    while (1)
    {
        printf("Waiting conection\n");
        connsockfd = accept(listening_socket, (struct sockaddr *)&client, &size_client);

        if (connsockfd < 0)
        {
            perror("Error accepting conection from client ");
            continue;
        }

        signal(SIGPIPE, SIG_IGN);
        printf("Conection accept\n");

        MessageT *msg;
        while ((msg = network_receive(connsockfd)) != NULL)
        {
            clock_t tempo = clock();
            if (invoke(msg) < 0)
            {
                perror("Erro ao ler dados");
                close(connsockfd);
                printf("Closed conection\n");
                continue;
            }

            if (network_send(connsockfd, msg) < 0)
            {
                close(connsockfd);
                printf("Closed conection\n");
                continue;
            }
            tempo = clock() - tempo;
            //meter na stats
        }
        // Fecha socket referente a esta conexão
        close(connsockfd);
        printf("Client exit: %d\n", connsockfd);
    }
    return 0;
}

/* Esta função deve:
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura MessageT.
 */
MessageT *network_receive(int client_socket)
{
    int buffer_len_recv;

    // Primeiro read recebe o unsigned len
    if (read(client_socket, &buffer_len_recv, sizeof(int)) <= 0)
    {
        perror("Erro ao receber dados do cliente");
        return NULL;
    }

    buffer_len_recv = ntohl(buffer_len_recv);
    uint8_t *recv_buf = malloc(buffer_len_recv);

    // Segundo read para ler a msg serializada
    if (read_all(client_socket, recv_buf, buffer_len_recv) <= 0)
    {
        return NULL;
    }

    MessageT *msg = NULL;
    msg = message_t__unpack(NULL, buffer_len_recv, recv_buf); // de-serializar msg

    free(recv_buf);
    return msg;
}

/* Esta função deve:
 * - Serializar a mensagem de resposta contida em msg;
 * - Libertar a memória ocupada por esta mensagem;
 * - Enviar a mensagem serializada, através do client_socket.
 */
int network_send(int client_socket, MessageT *msg)
{
    unsigned len, buffer_len;
    uint8_t *buf = NULL;

    len = message_t__get_packed_size(msg);
    buffer_len = htonl(len);

    buf = malloc(len);
    if (buf == NULL)
    {
        perror("malloc error\n");
        return -1;
    }

    message_t__pack(msg, buf);

    // envio do tamanho da msg
    if (write(client_socket, &buffer_len, sizeof(buffer_len)) < 0)
    {
        perror("Erro ao enviar dados para o cliente");
        return -1;
    }

    // envio da msg
    if (write_all(client_socket, buf, len) < 0)
    {
        perror("Erro ao enviar dados para o cliente");
        return -1;
    }

    message_t__free_unpacked(msg, NULL);
    free(buf);
    return 0;
}

/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close(int listening_socket)
{
    close(listening_socket);
    return 0;
}