/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

#include "client_stub.h"
#include "stats-private.h"
#include "client_stub-private.h"
#include "sdmessage.pb-c.h"

#define MAX_LEN 2048

struct rtable_t *table;
struct data_t *data;
struct entry_t *entry;

void closeLigacao(int num)
{

    if (!entry)
        entry_destroy(entry);
    
    if (!data)
        data_destroy(data);
    
    if (table)
        rtable_disconnect(table);
    
    exit(0);
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

    char input[MAX_LEN] = "";

    table = rtable_connect(argv[1]);
    if (!table)
        closeLigacao(0);
    
    do
    {
        while(table->primary_exists == 0) {
            printf("Trying connect to ther server...\n");
            sleep(1);
        }
        
        printf("\nComandos do Utilizador:\n");
        printf("  size\n");
        printf("  del <key>\n");
        printf("  get <key>\n");
        printf("  put <key> <data>\n");
        printf("  getkeys\n");
        printf("  table_print\n");
        printf("  stats\n");
        printf("  quit\n");
        printf("\nInput: ");
        signal(SIGINT, closeLigacao);

        char *erro = fgets(input, MAX_LEN, stdin);
        if (erro == NULL)
        {
            printf("Erro! Foi intruduzido uma string nao reconhecida\n");
            continue;
        }

        char *data_token;
        char *token = strtok(input, barra_n);

        token = strtok(input, espaco);
        char *key = strtok(NULL, espaco);
        data_token = strtok(NULL, espaco);

        if (!token)
        {
            printf("Erro! Nenhum comando foi dado\n");
        }
        else if (strcmp(token, "quit") == 0)
        {
            printf(" bye bye\n");
        }
        else if (strcmp(token, "size") == 0)
        {
            if (table == NULL)
                return -1;

            printf("  Size: %d\n", rtable_size(table));
        }
        else if (strcmp(token, "stats") == 0)
        {
            if (table == NULL)
                return -1;
            struct statistics *stats = rtable_stats(table);
            // todo fazer um display dos stats

            printf("  Stats:\n  Nº Puts:%d\n  Nº Gets:%d\n  Nº Deletes:%d\n  Nº Sizes:%d\
            \n  Nº GetKeys:%d\n  Nº TablePrints:%d\n  Tempo medio das operações:%lf\n", stats->nPuts, stats->nGets, 
            stats->nDels, stats->nSizes, stats->nGetKeys, stats->nTable_prints, stats->avg_per_operation);
            
            free(stats);
        }
        else if (strcmp(token, "table_print") == 0)
        {
            if (table == NULL)
                return -1;

            printf("  Table print:\n");
            rtable_print(table);
        }
        else if (strcmp(token, "getkeys") == 0)
        {
            if (table == NULL)
                return -1;

            printf("  Get keys:\n");
            char **chaves = rtable_get_keys(table);

            for (int i = 0; chaves[i] != NULL; i++)
            {
                printf("  %s\n", chaves[i]);
            }
            rtable_free_keys(chaves);
        }
        else if (strcmp(token, "del") == 0)
        {
            if (!key)
            {
                printf("Erro! Nao foi fornecida uma chave\n");
            }
            else
            {
                if (table == NULL)
                    return -1;

                if (rtable_del(table, key) == 0)
                    printf("  Delete \"%s\": Sucesso\n", key);
                else
                    printf("  Delete \"%s\": Insucesso\n", key);
            }
        }
        else if (strcmp(token, "get") == 0)
        {
            if (!key)
            {
                printf("Erro! Nao foi fornecida uma chave\n");
            }
            else
            {
                if (table == NULL)
                    return -1;

                data = rtable_get(table, key);
                printf("  Get \"%s\": ", key);

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
                data_destroy(data);
            }
        }
        else if (strcmp(token, "put") == 0)
        {
            if (!key)
            {
                printf("Erro! Nao foi fornecida uma chave\n");
            }
            else
            {
                if (!data_token)
                {
                    printf("Erro ! Data nao fornecida\n");
                }
                else
                {
                    if (table == NULL)
                        return -1;

                    //Criação da data e entry
                    int len = strlen(data_token);
                    data = data_create(len);
                    memcpy(data->data, data_token, len);
                    entry = entry_create(strdup(key), data);

                    if (rtable_put(table, entry) == 0)
                    {
                        printf("  put  \"%s ", key);
                        char temp;
                        for (int i = 0; i < data->datasize; i++)
                        {
                            memcpy(&temp, data->data + i, 1);
                            printf("%c", temp);
                        }
                        printf("\": Sucesso\n");
                    }
                    else
                    {
                        printf("  put  \"%s ", key);
                        char temp;
                        for (int i = 0; i < data->datasize; i++)
                        {
                            memcpy(&temp, data->data + i, 1);
                            printf("%c", temp);
                        }
                        printf("\": Insucesso\n");
                    }

                    entry_destroy(entry);
                }
            }
        }
        else
        {
            printf("Erro! Comando nao reconhecido\n");
        }

    } while (strcmp(input, "quit") != 0);
    rtable_disconnect(table);

    return 0;
}