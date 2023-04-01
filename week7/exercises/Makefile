CC = gcc
CFLAGS = -Wall -Werror
COMMON = Graph.c queue.c 
ALL = bfsQueue dfsRecur euler hamiltonian
.PHONY: all
all: bfsQueue dfsRecur euler hamiltonian

bfsQueue: bfsQueue.c $(COMMON)
	$(CC) $(CFLAGS) -o bfsQueue bfsQueue.c $(COMMON)
dfsRecur: dfsRecursive.c $(COMMON)
	$(CC) $(CFLAGS) -o dfsRecur dfsRecursive.c $(COMMON)
euler: euler.c $(COMMON)
	$(CC) $(CFLAGS) -o euler euler.c $(COMMON)
hamiltonian: hamiltonian.c $(COMMON)
	$(CC) $(CFLAGS) -o hamiltonian hamiltonian.c $(COMMON)
clean:
	rm -f $(ALL)