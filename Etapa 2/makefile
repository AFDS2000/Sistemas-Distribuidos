# Grupo 31 \
  André Cabaço 53457 \
  André Seixas 53870 \
  Miguel Agostinho 53475 \

SRC_DIR	= source
OBJ_DIR	= object
BIN_DIR = binary
PROTOC_DIR = /usr/local/

#client
table_client_objects = 
client_stub_objects = 
network_client_objects =

#server
table_server_objects = 
table_skel_objects = 
network_server_objects =

data.o = data.h
entry.o = data.h entry.h
list.o = data.h entry.h list.h list-private.h
table.o = data.h entry.h list.h list-private.h table.h table-private.h
table-private.o = table-private.h 
table_client.o =
client_stub.o = data.h entry.h client_stub.h
network_client.o = client_stub.h sdmessage.pb-c.h
table_server.o =
table_skel.o = sdmessage.pb-c.h table.h
network_server.o = table_skel.h

CC = gcc
CFLAGS = -Wall -g -O2 -I${PROTOC_DIR}include/ include/
LDFLAGS = ${PROTOC_DIR}lib/libprotobuf-c.a

vpath %.o $(OBJ_DIR)

%.o: $(SRC_DIR)/%.c $($@)
	$(CC) $(CFLAGS) -MD -o $(OBJ_DIR)/$@ -c $<

%.pb-c.c:%.proto
	${PROTOC_DIR}bin/protoc-c sdmessage.proto --c_out=./

all: table_client

table_client: $(table_client_objects)
	$(CC) $(addprefix $(OBJ_DIR)/,$(test_data_objects)) $(LDFLAGS) -o $(BIN_DIR)/$@

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*
