/* Grupo 31
 * André Cabaço 53457
 * André Seixas 53870
 * Miguel Agostinho 53475
 */

#ifndef _TABLE_SKEL_H
#define _TABLE_SKEL_H

#include "sdmessage.pb-c.h"
#include "table.h"

/* Inicia o skeleton da tabela.
 * O main() do servidor deve chamar esta função antes de poder usar a
 * função invoke(). O parâmetro n_lists define o número de listas a
 * serem usadas pela tabela mantida no servidor.
 * Retorna 0 (OK) ou -1 (erro, por exemplo OUT OF MEMORY)
 */
int table_skel_init(int n_lists);

/* Liberta toda a memória e recursos alocados pela função table_skel_init.
 */
void table_skel_destroy();

void zookeeper_destroy();

/* Executa uma operação na tabela (indicada pelo opcode contido em msg)
 * e utiliza a mesma estrutura MessageT para devolver o resultado.
 * Retorna 0 (OK) ou -1 (erro, por exemplo, tabela nao incializada)
*/
int invoke(MessageT *msg);

int zoo_init(char *ip_port, char *port);

#endif
