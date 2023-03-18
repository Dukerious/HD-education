CC = gcc
FLAGES = -Wall -Werror
recursive:	recursive.o
	$(CC) $(FLAGES) -o recursive recursive.o
recursive.o:	recursive.c recursive.h
	$(CC) $(FLAGES) -c recursive.c
clean:
	rm -f *.o recursive