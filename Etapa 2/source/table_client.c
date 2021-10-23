#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_stub.h"
int op_code(char string[])
{
    if (strcmp(string, "size") == 0)
        return 10;
    if (strcmp(string, "del") == 0)
        return 20;
    if (strcmp(string, "get") == 0)
        return 30;
    if (strcmp(string, "put") == 0)
        return 40;
    if (strcmp(string, "getkeys") == 0)
        return 50;
    if (strcmp(string, "print") == 0)
        return 60;
    if (strcmp(string, "quit") == 0)
        return 0;
    return -1;
}
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Erro na execução\n");
        printf("Execução do programa: ./<path_to_exe>/table.server <port> <n_lists>\n");
        return -1;
    }
    char *token;

    const char espaco[2] = " ";
    const char barra_n[2] = "\n";

    struct rtable_t *table = rtable_connect(argv[1]);

    char input[2048] = "";
    do
    {
        int n = 2048;
        char *erro = fgets(input, n, stdin);
        if (erro)
        {
            token = strtok(input, barra_n);
            token = strtok(token, espaco);
            int opCode = op_code(token);
            switch (opCode)
            {
            case 10:
                printf("\n  size  \n");
                int derp = rtable_size(table);

                break;

            case 20:
                printf("  delete  ");

                break;

            case 30:
                printf("  get  ");

                break;

            case 40:
                printf("  put  ");

                break;

            case 50:
                printf("  getkeys  ");

                break;

            case 60:
                printf("  print  ");

                break;
            case 0:
                printf(" bye bye ");
                break;
            default:
                printf("Erro! Comando nao reconhecido");
                return -1;
            }
        }
        else
        {
            printf("Erro! Foi intruduzido uma string nao reconhecida");
        }
    } while (strcmp(input, "quit") != 0);
    return 0;
}