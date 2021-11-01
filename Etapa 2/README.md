Grupo 31:
    André Cabaço 53457
    André Seixas 53870
    Miguel Agostinho 53475

IMPORTANTE: Para poder compilar os ficheiros em executáveis é necessário primeiro fazer o seguinte comando no terminal: "make sdmessage.pb-c.c".

O makefile do grupo consegue compilar todos os ficheiros em executáveis apenas escrevendo no terminal "make".
Caso seja necessário eliminar os ficheiros object (.o) ou os executáveis poderá escrever no terminal "make clean".
Caso seja necessário eliminar o ficheiro sdmessage.pb-c.c ou o ficheiro sdmessage.pb-c.h poderá escrever no terminal "make clean-sdmessage".


Foi criado o ficheiro client_stub-private.h que contém a struct rtable.
Foi criado o ficheiro message-private.c juntamente com o ficheiro message-private.h que contém a função read_all e a função write_all.
Foi criado o ficheiro table_skel-private.c juntamente com o ficheiro table_skel-private.h que contém as funções auxiliares onde estas ajudam a processar o pedido do cliente.
Foi criado o ficheiro inet-private.h que contém as bibliotecas para permitir criar uma ligação entre o cliente e o servidor.

O cliente ao realizar a operação "table_print", o grupo pensou em enviar para o cliente os dados em forma de listas e depois no lado do cliente seriam tratados para serem printados de forma correta no terminal para o cliente poder ver.
O código que fizemos para isto acontecer no ficheiro table_skel-private.c foi o seguinte:

```
void table_to_string(MessageT *msg, struct table_t *table)
{
    msg->opcode += 1;
    msg->c_type = MESSAGE_T__C_TYPE__CT_TABLE;

    int n_lists = table->n_lists;
    struct list_t **lists = table_print(table);

    msg->n_table = n_lists;
    msg->table = malloc(n_lists * sizeof(MessageT__Table *));

    MessageT__Table *table_temp = malloc(n_lists * sizeof(MessageT__Table));
    for (int i = 0; i < n_lists; i++)
    {
        // Inicia table_temp
        message_t__table__init(&table_temp[i]);

        //Lista das entries
        int n_entries = list_size(lists[i]);
        if (n_entries <= 0)
        {
            table_temp[i].n_entries = 0;
            table_temp[i].entries = NULL;
            msg->table[i] = &table_temp[i];
            free(&table_temp[i]);
            continue;
        }

        struct entry_t **entries = list_print(lists[i]);
        table_temp[i].n_entries = n_entries;
        table_temp[i].entries = malloc(n_entries * sizeof(MessageT__Table__Entry *));

        MessageT__Table__Entry *entry_temp = malloc(n_entries * sizeof(MessageT__Table__Entry));
        for (int j = 0; j < n_entries; j++)
        {
            // Inicia entry_temp
            message_t__table__entry__init(&entry_temp[j]);

            // escreve key na entry
            entry_temp[j].key = malloc(strlen(entries[j]->key) + 1);
            strcpy(entry_temp[j].key, entries[j]->key);

            // inicia dados na entry
            ProtobufCBinaryData data_temp;
            data_temp.len = entries[j]->value->datasize;
            data_temp.data = malloc(entries[j]->value->datasize);
            memcpy(data_temp.data, entries[j]->value->data, entries[j]->value->datasize);
            entry_temp[j].data = data_temp;

            table_temp[i].entries[j] = &entry_temp[j];
        }
        msg->table[i] = &table_temp[i];
    }
}

```

O nosso ficheiro .proto estava da seguinte maneira:

```
syntax = "proto3";
message message_t
 {
        enum Opcode {
                OP_BAD     = 0;
                OP_SIZE    = 10;
                OP_DEL     = 20;
                OP_GET     = 30;
                OP_PUT     = 40;
                OP_GETKEYS = 50;
                OP_PRINT   = 60;
                OP_ERROR   = 99;
        }
        Opcode opcode = 1;

        enum C_type {
                CT_BAD    = 0;
                CT_KEY    = 10;
                CT_VALUE  = 20;
                CT_ENTRY  = 30;
                CT_KEYS   = 40;
                CT_RESULT = 50;
                CT_TABLE  = 60;
                CT_NONE   = 70;
        }
        C_type c_type = 2;
        bytes data = 4;
        sint32 table_size = 5;
        repeated string keys = 6;

        message Table
        {
                message Entry
                {
                        string key = 1;
                        bytes data = 2;
                }
                repeated Entry entries = 1;
        }

        repeated Table table = 7;

};
```

Só que, ao fazermos "message_t__free_unpacked(msg, NULL);" na funcção network_send do ficheiro network_server.c para libertar a memória, dava o seguinte erro no terminal:
free(): invalid pointer
Abortado (núcleo despejado)

Este erro ficava resolvido se metessemos o "msg->tabel = NULL;" e assim já era mostrado no lado do cliente a informação da tabela, só que claro, não era libertada a memória.
Percebemos que provavelmente isto não irá contar para nota, mas gostariamos de compartilhar a solução mais eficaz que tentamos implementar.