# Grupo 31 \
  André Cabaço 53457 \
  André Seixas 53870 \
  Miguel Agostinho 53475 \

SRC_DIR	= source
OBJ_DIR	= object
BIN_DIR = binary
PROTOC_DIR = /usr/local/

#client
table_client_objects = table_client.o table-lib.o

#server
table_server_objects = data.o entry.o list.o table.o table-private.o table_server.o table_skel.o sdmessage.pb-c.o

data.o = data.h
entry.o = data.h entry.h
list.o = data.h entry.h list.h list-private.h
table.o = data.h entry.h list.h list-private.h table.h table-private.h
table-private.o = table-private.h 
table_client.o =
client_stub.o = 
network_client.o = 
table_server.o = data.h entry.h list.h list-private.h table.h table-private.h table_server.h table_skel.h sdmessage.pb-c.h
table_skel.o = data.h entry.h list.h list-private.h table.h table-private.h table_skel.h sdmessage.pb-c.h
network_server.o = network_server.h table_skel.h
sdmessage.pb-c.o = sdmessage.pb-c.h

CC = gcc
CFLAGS = -Wall -g -O2
LDFLAGS = ${PROTOC_DIR}lib/libprotobuf-c.a

vpath %.o $(OBJ_DIR)

%.o: $(SRC_DIR)/%.c $($@)
	$(CC) $(CFLAGS) -I include/ -o $(OBJ_DIR)/$@ -c $<

%.pb-c.o: $(SRC_DIR)/%.pb-c.c $($@)
	$(CC) $(CFLAGS) -I include/ -o $(OBJ_DIR)/$@ -c $<
	
%.pb-c.c:%.proto
	${PROTOC_DIR}bin/protoc-c sdmessage.proto --c_out=./
	mv sdmessage.pb-c.c $(SRC_DIR)/
	mv sdmessage.pb-c.h include/

#library
client-lib.o:
	ld -r (objetos que estao no pdf) -o client-lib.o

all: table_client table_server

table-client: $(table_client_objects)
	$(CC) $(addprefix $(OBJ_DIR)/,$(table_client_objects)) -I $(PROTOC_DIR)include -L $(PROTOC_DIR)lib -lprotobuf-c -o $(BIN_DIR)/$@

table-server: $(table_server_objects)
	$(CC) $(addprefix $(OBJ_DIR)/,$(table_server_objects)) -I $(PROTOC_DIR)include -L $(PROTOC_DIR)lib -lprotobuf-c -o $(BIN_DIR)/$@

clean:
	rm -f $(OBJ_DIR)/*.o && rm -f $(OBJ_DIR)/*.d
	rm -f $(BIN_DIR)/*