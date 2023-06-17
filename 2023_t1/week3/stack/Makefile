CC = gcc
FLAGES = -Wall -Werror
tests:	tests.o		stack.o 
	$(CC) $(FLAGES) -o tests tests.o stack.o

stack.o:	stack.c stack.h
	$(CC) $(FLAGES) -c stack.c

tests.o:	tests.c stack.h
	$(CC) $(FLAGES) -c tests.c

clean:
	rm -f *.o tests