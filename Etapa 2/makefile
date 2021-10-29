# Grupo 31 \
  André Cabaço 53457 \
  André Seixas 53870 \
  Miguel Agostinho 53475 \

SRC_DIR	= source
OBJ_DIR	= object
BIN_DIR = binary
PROTOC_DIR = /usr/local/

#client
table_client_objects = data.o entry.o list.o table.o table-private.o table_client.o client_stub.o network_client.o sdmessage.pb-c.o message-private.o 

#server
table_server_objects = data.o entry.o list.o table.o table-private.o table_server.o network_server.o table_skel.o table_skel-private.o sdmessage.pb-c.o message-private.o

data.o = data.h
entry.o = entry.h
list.o = list.h list-private.h
table.o = list.h table.h table-private.h
table-private.o = table-private.h 

sdmessage.pb-c.o = sdmessage.pb-c.h
message-private.o = message-private.h

table_client.o = data.h client_stub.h client_stub-private.h 
client_stub.o = table.h client_stub.h client_stub-private.h sdmessage.pb-c.h message-private.h
network_client.o = network_client.h client_stub-private.h message-private.h

table_server.o = table_skel.h network_server.h
network_server.o = table.h message-private.h inet-private.h table_skel.h network_server.h sdmessage.pb-c.h
table_skel.o = table_skel.h table_skel-private.h
table_skel-private.o = table_skel-private.h

CC = gcc
CFLAGS = -Wall -g -O2
LDFLAGS = ${PROTOC_DIR}lib/libprotobuf-c.a

vpath %.o $(OBJ_DIR)

%.o: $(SRC_DIR)/%.c $($@)
	$(CC) $(CFLAGS) -I include/ -o $(OBJ_DIR)/$@ -c $<

all: table-client table-server

%.pb-c.o: $(SRC_DIR)/%.pb-c.c $($@)
	$(CC) $(CFLAGS) -I include/ -o $(OBJ_DIR)/$@ -c $<
	
sdmessage.pb-c.c: sdmessage.proto
	${PROTOC_DIR}bin/protoc-c sdmessage.proto --c_out=./
	mv sdmessage.pb-c.c $(SRC_DIR)/
	mv sdmessage.pb-c.h include/

#library
client-lib.o:
	ld -r client_stub.o -o client-lib.o

table-client: $(table_client_objects)
	$(CC) $(addprefix $(OBJ_DIR)/,$(table_client_objects)) -I $(PROTOC_DIR)include -L $(PROTOC_DIR)lib -lprotobuf-c -o $(BIN_DIR)/$@

table-server: $(table_server_objects)
	$(CC) -fno-stack-protector $(addprefix $(OBJ_DIR)/,$(table_server_objects)) -I $(PROTOC_DIR)include -L $(PROTOC_DIR)lib -lprotobuf-c -o $(BIN_DIR)/$@

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*

clean-sdmessage:
	rm -f $(SRC_DIR)/*.pb-c.c
	rm -f $(SRC_DIR)/*.pb-c.h