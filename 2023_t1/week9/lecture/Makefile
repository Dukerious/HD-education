CC = gcc
CFLAGS = -Wall -Werror

sort: sorting.o
	$(CC) $(CFLAGS) -o sort sorting.o
sorting.o: sorting.c
	$(CC) $(CFLAGS) -c sorting.c
clean:
	rm -f *.o sort