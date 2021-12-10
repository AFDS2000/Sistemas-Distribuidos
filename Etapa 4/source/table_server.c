/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#include "table_skel.h"
#include "network_server.h"
#include "stats_server-private.h"

int server_socket;

void closeSocket(int num)
{
    network_server_close(server_socket);
    table_skel_destroy(); // destroir a tabela
    stats_server_destroy();
    zookeeper_destroy();
    exit(0);
}

int main(int argc, char const *argv[])
{
    if (argc != 4)
    {
        printf("Erro na execução\n");
        printf("Uso: ./<path_to_exe>/table-server <port> <n_lists>\n");
        printf("Exemplo de uso: ./binary/table-server 12345 5\n");
        return -1;
    }

    int err;
    short port = atoi(argv[1]);
    int n_lists = atoi(argv[2]);
    if (zoo_init((char *)argv[3], (char *)argv[1]) != 0)
    {
        closeSocket(0);
    }

    printf("Port: %d\nN-lists: %d\n", port, n_lists);

    server_socket = network_server_init(port);
    signal(SIGINT, closeSocket);

    if (server_socket < 0)
    {
        printf("Erro ao preprar o socket");
        return -1;
    }

    // processo de verificar primary, backup e depois escolher ou fechar o programa

    // inicializar a table
    err = table_skel_init(n_lists);
    if (err < 0)
    {
        printf("Erro ao criar a tabela");
        network_server_close(server_socket);
        return err;
    }
    stats_server_init();

    network_main_loop(server_socket);

    return 0;
}