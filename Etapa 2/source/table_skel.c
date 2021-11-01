/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#include <stdio.h>
#include <stdlib.h>

#include "table_skel.h"
#include "table_skel-private.h"

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
int invoke(MessageT *msg)
{
    if (table == NULL || msg == NULL)
        return -1;

    switch (msg->opcode)
    {
    case 10:
        get_table_size(msg, table);
        break;
    case 20:
        del_entry(msg, table);
        break;
    case 30:
        get_entry(msg, table);
        break;
    case 40:
        put_entry(msg, table);
        break;
    case 50:
        get_keys(msg, table);
        break;
    case 60:
        table_to_string(msg, table);
        break;
    default:
        msg->opcode = 99;
        msg->c_type = 70;
    }
    return 0;
}