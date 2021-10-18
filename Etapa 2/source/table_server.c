#include <stdio.h>
#include <stdlib.h>

#include "table_skel.h"
#include "table-private.h"

int main(int argc, char const *argv[])
{
    if(argc < 3) {
        printf("Erro na execução\n");
        printf("Execução do programa: ./<path_to_exe>/table.server <port> <n_lists>\n");
        return -1;
    }

    int port = atoi(argv[1]);
    int n_lists = atoi(argv[2]);

    printf("Port: %d\nN-lists: %d\n", port, n_lists);

    //inicializar a table
    table_skel_init(n_lists);
    return 0;
}
