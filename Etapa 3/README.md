Grupo 31:
    André Cabaço 53457
    André Seixas 53870
    Miguel Agostinho 53475

IMPORTANTE: Para poder compilar os ficheiros em executáveis é necessário primeiro fazer o seguinte comando no terminal: "make sdmessage.pb-c.c".

O makefile do grupo consegue compilar todos os ficheiros em executáveis apenas escrevendo no terminal "make".
Caso seja necessário eliminar os ficheiros object (.o) ou os executáveis poderá escrever no terminal "make clean".
Caso seja necessário eliminar o ficheiro sdmessage.pb-c.c ou o ficheiro sdmessage.pb-c.h poderá escrever no terminal "make clean-sdmessage".

### Relatório

No projeto da etapa 2 foram indentificado dois pontos a melhorar:<br />
    - No protobuf, não empregaram plenamente os recursos que este framework proporciona. Não foi utilizado o recurso repeated para serialização/de-serialização de entries. Diferente da técnica ensinada em aula, o envio da table do cliente para o servidor foi feito em uma string.<br />
    - Não empregam write_all e read_all na transmissão e receção do tamanho das mensagens (tanto no client quanto no server). Mesmo sendo pequeno o tamanho do dado transmitido/recebido nesta etapa, é prudente utilizar as referidas funções ensinadas em aula.<br />

Estes pontos já foram revistos e tratados.<br />

Relativamente à etapa 3, foram criados os seguintes ficheiros: <br />
    1º stats-private.h: Este ficheiro contém a struct statistics que reprensenta a estatistica dos pedidos dos clientes ao servidor.<br />
    2º stats_server-private.h / stats_server-private.c: Este ficheiro inicializa e destroy a estrutura que guarda as estatisticas referentes ao servidor. Também permite     atualizar as estatisticas uando ocorre algum pedido vindo do cliente e também permite obter as estatisticas do servidor.<br />
    3º ctrl_mutex-private.h / ctrl_mutex-private.h: Este ficheiro permite fazer controlo da concorrencia da hash table e da estrutura que guarda as estatisticas. Para tratarmos da concorrência da hash table, o grupo baseou-se dos slides de Sistemas Operativos, relativamente ao problema dos Leitores/Escritores. Uma vez que o código de exemplo que está nos slides não se preocupa com a ordem de chegada do pedido, ou seja, caso esteja algum escritor à espera e novos leitores cheguem depois, estes leitores vão passar à "frente" do escritor que já estava à mais tempo à espera. Por isso adpatamos este código com o exemplo dos professores, ou seja, fazer uma fila de espera. Por tanto, o nosso código permite que vários utilizadores leiam ao mesmo tempo, bloqueando os escritores, mas caso cheguem novos pedidos para ler, estes não irão passar à "frente" dos escritores que já estão na fila de espera. Caso algum escritor esteja a escrever, nenhum leitor conseguirá ler os dados da hash table. Para o caso da concorrência da estrutura que guarda as estatisticas, apenas fazemos um lock e unlock tanto para o update como para o getStats();<br />

Também fizemos alteração de alguns ficheiros das etapas anterios, nomeadamente:<br />
    1º client_stub.h / client_stub.h: Foi acrescentado a função getStats(), onde este faz o pedido da estrutura que guarda as estatisticas do servidor.<br />
    2º table_skel.h / table_skel.c: Foi reajustada a assinatura da função invoke(). Esta passou a receber, para além da Message_t também um inteiro que representa o ticket da   thread<br />
    3º network_server.c: Foi adicionado a função private thread_main_loop() que é invocada quando uma thread é criada na função main_loop(). A função main_loop(), como obvio foi alterada devido ao que já foi explicado anteriormente.
