#include <stdio.h>
#include <stdlib.h>

#include "network_client.h"
#include "client_stub-private.h"

/* Esta função deve:
 * - Obter o endereço do servidor (struct sockaddr_in) a base da
 *   informação guardada na estrutura rtable;
 * - Estabelecer a ligação com o servidor;
 * - Guardar toda a informação necessária (e.g., descritor do socket)
 *   na estrutura rtable;
 * - Retornar 0 (OK) ou -1 (erro).
 */
int network_connect(struct rtable_t *rtable)
{
    if ((rtable->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Erro ao criar socket TCP");
        return -1;
    }
    rtable->addr.sin_family = AF_INET;           // família de endereços
    rtable->addr.sin_port = htons(rtable->port); // Porta TCP
    if (inet_pton(AF_INET, rtable->ip, &rtable->addr.sin_addr) < 1)
    {
        printf("Erro ao converter IP\n");
        close(rtable->socket);
        return -1;
    }
    if (connect(rtable->socket, (struct sockaddr *)&rtable->addr, sizeof(rtable->addr)) < 0)
    {
        perror("Erro ao conectar-se ao servidor");
        close(rtable->socket);
        return -1;
    }
    return 0;
}

/* Esta função deve:
 * - Obter o descritor da ligação (socket) da estrutura rtable_t;
 * - Reservar memória para serializar a mensagem contida em msg;
 * - Serializar a mensagem contida em msg;
 * - Enviar a mensagem serializada para o servidor;
 * - Libertar a memória ocupada pela mensagem serializada enviada;
 * - Esperar a resposta do servidor;
 * - Reservar a memória para a mensagem serializada recebida;
 * - De-serializar a mensagem de resposta, reservando a memória 
 *   necessária para a estrutura MessageT que é devolvida;
 * - Libertar a memória ocupada pela mensagem serializada recebida;
 * - Retornar a mensagem de-serializada ou NULL em caso de erro.
 */
MessageT *network_send_receive(struct rtable_t *rtable, MessageT *msg);

/* A função network_close() fecha a ligação estabelecida por
 * network_connect().
 */
int network_close(struct rtable_t *rtable);