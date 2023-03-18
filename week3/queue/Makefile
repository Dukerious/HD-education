CC = gcc
FLAGES = -Wall -Werror
testq:	testq.o		queue.o 
	$(CC) $(FLAGES) -o testq testq.o queue.o

queue.o:	queue.c queue.h
	$(CC) $(FLAGES) -c queue.c

testq.o:	testq.c queue.h
	$(CC) $(FLAGES) -c testq.c

clean:
	rm -f *.o testq