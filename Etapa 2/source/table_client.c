#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_stub.h"

#define MAX_LEN 2048

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
        printf("Uso: ./<path_to_exe>/table-client <address>:<port>\n");
        printf("Exemplo de uso: ./binary/table-client 127.0.0.1:12345\n");
        return -1;
    }

    const char espaco[2] = " ";
    const char barra_n[2] = "\n";

    struct rtable_t *table = rtable_connect(argv[1]);
    if (table == NULL)
        return -1;

    char input[MAX_LEN] = "";
    do
    {
        system("clear");
        printf("Comandos do Utilizador:\n");
        printf("  size\n");
        printf("  del <key>\n");
        printf("  get <key>\n");
        printf("  put <key> <data>\n");
        printf("  getkeys\n");
        printf("  table_print\n");
        printf("  quit\n");
        printf("\nInput: ");
        
        char *erro = fgets(input, MAX_LEN, stdin);
        if (erro == NULL)
        {
            printf("Erro! Foi intruduzido uma string nao reconhecida\n");
            continue;
        }

        char *token = strtok(input, barra_n);
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
            printf("Erro! Comando nao reconhecido\n");
        }

        fflush(stdin);
        printf("Prima ENTER para continuar\n");
        getchar();
        fflush(stdin);

    } while (strcmp(input, "quit") != 0);
    return 0;
}