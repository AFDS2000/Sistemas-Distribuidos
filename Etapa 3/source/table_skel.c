/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>

#include "table_skel.h"
#include "table_skel-private.h"
#include "stats_server-private.h"
#include "ctrl_mutex-private.h"

static struct table_t *table;

/* Inicia o skeleton da tabela.
 * O main() do servidor deve chamar esta função antes de poder usar a
 * função invoke(). O parâmetro n_lists define o número de listas a
 * serem usadas pela tabela mantida no servidor.
 * Retorna 0 (OK) ou -1 (erro, por exemplo OUT OF MEMORY)
 */
int table_skel_init(int n_lists)
{
    table = table_create(n_lists);
    return table ? 0 : -1;
}

/* Liberta toda a memória e recursos alocados pela função table_skel_init.
 */
void table_skel_destroy()
{
    table_destroy(table);
}

/* Executa uma operação na tabela (indicada pelo opcode contido em msg)
 * e utiliza a mesma estrutura MessageT para devolver o resultado.
 * Retorna 0 (OK) ou -1 (erro, por exemplo, tabela nao incializada)
*/
int invoke(MessageT *msg, int ticket)
{
    if (table == NULL || msg == NULL)
        return -1;

    switch (msg->opcode)
    {
    case MESSAGE_T__OPCODE__OP_SIZE:
        lock_read(ticket);
        get_table_size(msg, table);
        unlock_read();
        break;
    case MESSAGE_T__OPCODE__OP_DEL:
        lock_write(ticket);
        del_entry(msg, table);
        unlock_write();
        break;
    case MESSAGE_T__OPCODE__OP_GET:
        lock_read(ticket);
        get_entry(msg, table);
        unlock_read();
        break;
    case MESSAGE_T__OPCODE__OP_PUT:
        lock_write(ticket);
        put_entry(msg, table);
        unlock_write();
        break;
    case MESSAGE_T__OPCODE__OP_GETKEYS:
        lock_read(ticket);
        get_keys(msg, table);
        unlock_read();
        break;
    case MESSAGE_T__OPCODE__OP_PRINT:
        lock_read(ticket);
        table_to_string(msg, table);
        unlock_read();
        break;
    case MESSAGE_T__OPCODE__OP_STATS:
        lock_stats();
        get_stats(msg, getStats());
        unlock_stats();
        break;
    default:
        msg->opcode = MESSAGE_T__OPCODE__OP_ERROR;
        msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;
    }

    return 0;
}