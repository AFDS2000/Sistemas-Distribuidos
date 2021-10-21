#include <stdio.h>
#include <stdlib.h>

#include "network_server.h"
#include "inet.h"

/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar descritor do socket (OK) ou -1 (erro).
 */
int network_server_init(short port){
    int socketfd;
    struct sockaddr_in server;

    if((socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){ //falta proto
        return -1;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(atoi(argv[1])); //passa porto da conect TCP no arg
    server.sin_addr.s:addr = htons(INADDR_ANY);

    //bind
    if(bind(socketfd, (struct sockaddr *) &server, sizeof(server)) < 0){
        closer(socketfd);
        return -1;
    }

    //listen
    if (listen(socketfd, 0) < 0){
        close(socketfd);
        return -1;
    }
    
}

/* Esta função deve:
 * - Aceitar uma conexão de um cliente;
 * - Receber uma mensagem usando a função network_receive;
 * - Entregar a mensagem de-serializada ao skeleton para ser processada;
 * - Esperar a resposta do skeleton;
 * - Enviar a resposta ao cliente usando a função network_send.
 */
int network_main_loop(int listening_socket){

}

/* Esta função deve:
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura MessageT.
 */
MessageT *network_receive(int client_socket);

/* Esta função deve:
 * - Serializar a mensagem de resposta contida em msg;
 * - Libertar a memória ocupada por esta mensagem;
 * - Enviar a mensagem serializada, através do client_socket.
 */
int network_send(int client_socket, MessageT *msg);

/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close();