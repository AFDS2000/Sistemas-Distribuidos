SRC_dir	= source
OBJ_dir	= object
BIN_dir = binary

test_data_objects = data.o test_data.o

data.o = data.h
test_data.o = data.h

CC = gcc -g
CFLAGS = -Wall -lrt -lpthread

vpath %.o $(OBJ_dir)

test_data: $(test_data_objects)
	$(CC) $(CFLAGS) $(addprefix $(OBJ_dir)/,$(test_data_objects)) -o $(BIN_dir)/test_data 

%.o: $(SRC_dir)/%.c $($@)
	$(CC) -I include -o $(OBJ_dir)/$@ -c $<

test_data_debbug:
	valgrind --leak-check=full ./binary/test_data

clean:
	rm -f $(OBJ_dir)/*.o
	rm -f $(BIN_dir)/*