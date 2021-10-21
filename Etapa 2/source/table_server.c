#include <stdio.h>
#include <stdlib.h>

#include "table_skel.h"
#include "network_server.h"
#include "sdmessage.pb-c.h"

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Erro na execução\n");
        printf("Uso: ./<path_to_exe>/table.server <port> <n_lists>\n");
        printf("Exemplo de uso: ./binary/table.server 9393 5\n");
        return -1;
    }
    
    int err;
    short port = atoi(argv[1]);
    int n_lists = atoi(argv[2]);

    printf("Port: %d\nN-lists: %d\n", port, n_lists);

    // err = network_server_init(port);

    // inicializar a table
    err = table_skel_init(n_lists);
    if (err != 0)
        return err;

    MessageT msg;
    message_t__init(&msg);

    msg.opcode = 10;
    msg.data = NULL;
    invoke(&msg);

    printf("data: %d : %s\n", msg.data_size, msg.data);

    // destroir a table
    table_skel_destroy();
    free(msg.data);
    return 0;
}