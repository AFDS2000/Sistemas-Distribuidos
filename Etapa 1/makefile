SRC_dir	= source
OBJ_dir	= object
BIN_dir = binary

test_data_objects = data.o test_data.o
test_entry_objects = data.o entry.o test_entry.o
test_list_objects = data.o entry.o list.o list-private.o test_list.o

data.o = data.h
entry.o = data.h entry.h
list.o = data.h entry.h list.h list-private.h
list-private.o = data.h entry.h list.h list-private.h
test_data.o = data.h
test_entry.o = data.h entry.h
test_list.o = data.h entry.h list.h list-private.h


CC = gcc -g
CFLAGS = -Wall -lrt -lpthread

vpath %.o $(OBJ_dir)

%.o: $(SRC_dir)/%.c $($@)
	$(CC) -I include -o $(OBJ_dir)/$@ -c $<

test_data: $(test_data_objects)
	$(CC) $(CFLAGS) $(addprefix $(OBJ_dir)/,$(test_data_objects)) -o $(BIN_dir)/test_data 

test_entry: $(test_entry_objects)
	$(CC) $(CFLAGS) $(addprefix $(OBJ_dir)/,$(test_entry_objects)) -o $(BIN_dir)/test_entry 

test_list: $(test_list_objects)
	$(CC) $(CFLAGS) $(addprefix $(OBJ_dir)/,$(test_list_objects)) -o $(BIN_dir)/test_list

test_data_debbug:
	valgrind --leak-check=full ./binary/test_data

test_entry_debbug:
	valgrind --leak-check=full ./binary/test_entry

clean:
	rm -f $(OBJ_dir)/*.o
	rm -f $(BIN_dir)/*
