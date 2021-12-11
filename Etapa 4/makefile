# Grupo 31 \
  André Cabaço 53457 \
  André Seixas 53870 \
  Miguel Agostinho 53475 \

SRC_DIR	= source
OBJ_DIR	= object
BIN_DIR = binary
PROTOC_DIR = /usr/local/

#client
table_client_objects = table_client.o client-lib.o

#server
table_server_objects = data.o entry.o list.o table.o table-private.o table_server.o network_server.o \
	table_skel.o table_skel-private.o sdmessage.pb-c.o message-private.o stats_server-private.o zookeeper_server-private.o

#client-lib
cliet_lib_objects = data.o entry.o list.o table.o table-private.o client_stub.o network_client.o \
	message-private.o sdmessage.pb-c.o zookeeper_client-private.o

data.o = data.h
entry.o = entry.h
list.o = list.h list-private.h
table.o = list.h table.h table-private.h
table-private.o = table-private.h 

sdmessage.pb-c.o = sdmessage.pb-c.h
message-private.o = message-private.h

table_client.o = data.h client_stub.h client_stub-private.h 
client_stub.o = table.h client_stub.h client_stub-private.h sdmessage.pb-c.h message-private.h \
	stats-private.h zookeeper_client-private.h
network_client.o = network_client.h client_stub-private.h message-private.h

table_server.o = table_skel.h network_server.h stats_server-private.o zookeeper_server-private.o
network_server.o = table.h message-private.h inet-private.h table_skel.h network_server.h \
	sdmessage.pb-c.h stats_server-private.o
table_skel.o = table_skel.h table_skel-private.h stats_server-private.h zookeeper_server-private.h
table_skel-private.o = table_skel-private.h stats_server-private.h zookeeper_server-private.h network_server.h

stats_server-private.o = states_server-private.h stats-private.h sdmessage.pb-c.h
zookeeper_server-private.o = zookeeper_server-private.h
zookeeper_client-private.o = zookeeper_client-private.h client_stub.h

CC = gcc 
CFLAGS = -Wall -g -O0 -D THREADED
LDFLAGS = ${PROTOC_DIR}lib/libprotobuf-c.a 

vpath %.o $(OBJ_DIR)

%.o: $(SRC_DIR)/%.c $($@)
	$(CC) $(CFLAGS) -I include/ -o $(OBJ_DIR)/$@ -c $<

all: table-client table-server 
	
sdmessage.pb-c.c: sdmessage.proto
	${PROTOC_DIR}bin/protoc-c sdmessage.proto --c_out=./
	mv sdmessage.pb-c.c $(SRC_DIR)/
	mv sdmessage.pb-c.h include/

#library
client-lib.o: $(cliet_lib_objects)
	ld -r $(OBJ_DIR)/data.o $(OBJ_DIR)/entry.o $(OBJ_DIR)/list.o $(OBJ_DIR)/table.o $(OBJ_DIR)/table-private.o $(OBJ_DIR)/client_stub.o $(OBJ_DIR)/network_client.o $(OBJ_DIR)/message-private.o $(OBJ_DIR)/sdmessage.pb-c.o $(OBJ_DIR)/zookeeper_client-private.o -o lib/client-lib.o
	
table-client: $(table_client_objects)
	$(CC) $(OBJ_DIR)/table_client.o lib/client-lib.o -I $(PROTOC_DIR)include -L $(PROTOC_DIR)lib -lprotobuf-c -lzookeeper_mt -o $(BIN_DIR)/$@

table-server: $(table_server_objects)
	$(CC) $(addprefix $(OBJ_DIR)/,$(table_server_objects)) -I $(PROTOC_DIR)include -L $(PROTOC_DIR)lib -pthread -lprotobuf-c -lzookeeper_mt -o $(BIN_DIR)/$@

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*
	rm -f lib/*

clean-sdmessage:
	rm -f $(SRC_DIR)/*.pb-c.c
	rm -f $(SRC_DIR)/*.pb-c.h