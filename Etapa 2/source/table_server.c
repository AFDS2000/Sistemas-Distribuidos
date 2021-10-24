#include <stdio.h>
#include <stdlib.h>

#include "table_skel.h"
#include "network_server.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Erro na execução\n");
        printf("Uso: ./<path_to_exe>/table-server <port> <n_lists>\n");
        printf("Exemplo de uso: ./binary/table-server 12345 5\n");
        return -1;
    }

    int err;
    short port = atoi(argv[1]);
    int n_lists = atoi(argv[2]);

    printf("Port: %d\nN-lists: %d\n", port, n_lists);

    int listening_socket = network_server_init(port);
    if (listening_socket < 0)
    {
        printf("Erro ao preprar o socket");
        return -1;
    }

    // inicializar a table
    err = table_skel_init(n_lists);
    if (err < 0) {
        printf("Erro ao criar a tabela");
        network_server_close();
        return err;

    }
        
    network_main_loop(listening_socket);

    // destroir a table
    table_skel_destroy();

    return 0;
}