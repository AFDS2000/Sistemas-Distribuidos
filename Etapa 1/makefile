sc	= source/
in	= include/
ob	= object/
bin = binary/
fl	=  -Wall -I include -g

test_data: $(ob)data.o $(ob)test_data.o
	gcc $(fl) $(ob)data.o $(ob)test_data.o -o tdata -lrt -lpthread -g

$(ob)test_data.o: $(in)data.h $(sc)test_data.c
	gcc -c $(fl) $(sc)test_data.c -o $(ob)test_data.o

$(ob)data.o: $(in)data.h $(sc)data.c
	gcc -c $(fl) $(sc)data.c -o $(ob)data.o

clean:
	rm -f $(ob)data.o $(ob)teste_data.o tdata
