CC = gcc
CFLAGS = -Wall -Werror

sort: resorting.o
	$(CC) $(CFLAGS) -o resort resorting.o
sorting.o: sorting.c
	$(CC) $(CFLAGS) -c resorting.c
clean:
	rm -f *.o resort