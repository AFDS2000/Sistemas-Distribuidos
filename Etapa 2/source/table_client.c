#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client_stub.h"
#include "client_stub-private.h"
#include "sdmessage.pb-c.h"

#define MAX_LEN 2048

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

    char input[MAX_LEN] = "";
    do
    {
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
        struct rtable_t *table;
        struct data_t *data;
        struct entry_t *entry;
        char *data_token;
        char *token = strtok(input, barra_n);

        token = strtok(input, espaco);
        char *key = strtok(NULL, espaco);

        data_token = strtok(NULL, espaco);
        if (strcmp(token, "size") == 0)
        {
            table = rtable_connect(argv[1]);
            if (table == NULL)
                return -1;

            printf("\n  size: %d  \n", rtable_size(table));
            rtable_disconnect(table);
        }
        else if (strcmp(token, "del") == 0)
        {
            table = rtable_connect(argv[1]);
            if (table == NULL)
                return -1;

            printf("  delete  %d", rtable_del(table, key));
            rtable_disconnect(table);
        }
        else if (strcmp(token, "get") == 0)
        {
            table = rtable_connect(argv[1]);
            if (table == NULL)
                return -1;

            data = rtable_get(table, key);
            printf("  get: ");

            if (data && data->data)
            {
                char temp;
                for (int i = 0; i < data->datasize; i++)
                {
                    memcpy(&temp, data->data + i, 1);
                    printf("%c", temp);
                }
            }
            printf("\n");
            rtable_disconnect(table);
            data_destroy(data);
        }
        else if (strcmp(token, "put") == 0)
        {
            table = rtable_connect(argv[1]);
            if (table == NULL)
                return -1;

            int len = strlen(data_token);
            data = data_create(len);
            memcpy(data->data, data_token, len);

            entry = entry_create(strdup(key), data);
            printf("  put  %d \n", rtable_put(table, entry));
            rtable_disconnect(table);
            entry_destroy(entry);
        }
        else if (strcmp(token, "getkeys") == 0)
        {
            table = rtable_connect(argv[1]);
            if (table == NULL)
                return -1;

            printf("  getkeys  \n");
            char **chaves = rtable_get_keys(table);

            for (int i = 0; chaves[i] != NULL; i++)
            {
                printf("%s\n", chaves[i]);
            }
            rtable_disconnect(table);
            rtable_free_keys(chaves);
        }

        else if (strcmp(token, "print") == 0)
        {
            table = rtable_connect(argv[1]);
            if (table == NULL)
                return -1;
            printf("  print  ");
            rtable_disconnect(table);
        }
        else if (strcmp(token, "quit") == 0)
        {
            printf(" bye bye \n");
        }
        else
        {
            printf("Erro! Comando nao reconhecido\n");
        }

    } while (strcmp(input, "quit") != 0);
    return 0;
}