Grupo 31:
    André Cabaço 53457
    André Seixas 53870
    Miguel Agostinho 53475

IMPORTANTE: Para poder compilar os ficheiros em executáveis é necessário primeiro fazer o seguinte comando no terminal: "make sdmessage.pb-c.c".

O makefile do grupo consegue compilar todos os ficheiros em executáveis apenas escrevendo no terminal "make".
Caso seja necessário eliminar os ficheiros object (.o) ou os executáveis poderá escrever no terminal "make clean".
Caso seja necessário eliminar o ficheiro sdmessage.pb-c.c ou o ficheiro sdmessage.pb-c.h poderá escrever no terminal "make clean-sdmessage".

Foi criado o ficheiro zookeeper_server-private.c juntamente com o ficheiro zookeeper_server-private.h que contém a funções:
    init_zookeeper: tratam de iniciar a ligação ao zookeeper, establecer o znode kvstore e os filhos;
    zoo_destroy: termina a ligação e elementos allocados durante o funcionamento do zookeeper;
    backup_client: establece a ligação ao servidor backup em caso de ser primary;
    connection_watcher e child_watcher: watchers para observar alterações no zookeeper e reagir de forma apropriada;

Foi criado o ficheiro zookeeper_client-private.c juntamente com o ficheiro zookeeper_client-private.h que contém a funções:
    init_zookeeper:tratam de iniciar a ligação ao zookeeper e ir buscar o ip:porto do servidor primario;
    connection_watcher e child_watcher: watchers para observar alterações no zookeeper e reagir de forma apropriada;    
    zoo_destroy: termina a ligação ao zookeeper;