// Implementation of the Queue ADT using a linked list

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

typedef struct node *Node;
struct node {
	Item item;
	Node next;
};

struct queue {
	Node head;
	Node tail;
	int  size;
};

/**
 * Creates a new empty queue
 */
Queue QueueNew(void) {
	Queue q = malloc(sizeof(*q));
	if (q == NULL) {
		fprintf(stderr, "couldn't allocate Queue\n");
		exit(EXIT_FAILURE);
	}

	q->head = NULL;
	q->tail = NULL;
	q->size = 0;
	return q;
}

/**
 * Frees all resources associated with the given queue
 */
void QueueFree(Queue q) {
	Node curr = q->head;
	while (curr != NULL) {
		Node temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(q);
}

/**
 * Adds an item to the end of the queue
 */
void QueueEnqueue(Queue q, Item it) {
	// we should malloc a new node space called enqueue.
	Node enqueue = malloc(sizeof (struct node));
	// construct this new node.
	enqueue -> item = it;
	enqueue -> next = NULL;
	// situation 1: if there is nothing node in the queue,
	// we make head and tail both point to the new one. 
	if (q -> head == NULL) {
		q -> head = enqueue;
		q -> tail = enqueue;
		q -> size = (q -> size) + 1;
		return;
	}
	// situation 2: if it's the normal case,
	// we can find the last node and make it point to the 
	// new node which is we just constructed, and update the size 
	// and tail information for the queue q.
	q -> tail -> next = enqueue;
	q -> tail = enqueue;
	q -> size = (q -> size) + 1;
}

/**
 * Removes an item from the front of the queue and returns it
 * Assumes that the queue is not empty
 */
Item QueueDequeue(Queue q) {
	// find the head which we want to remove.
	Node dequeue = q -> head;
	// make sure update the head and size for the q.
	q -> head = dequeue -> next;
	q -> size = (q -> size) - 1;
	// find return item value.
	Item dequeueitem = dequeue -> item;
	// free the dequeue avoid the memory leak.
	free(dequeue); 
	return dequeueitem;
}

/**
 * Gets the item at the front of the queue without removing it
 * Assumes that the queue is not empty
 */
Item QueueFront(Queue q) {
	assert(q->size > 0);

	return q->head->item;
}

/**
 * Gets the size of the given queue
 */
int QueueSize(Queue q) {
	return q->size;
}

/**
 * Returns true if the queue is empty, and false otherwise
 */
bool QueueIsEmpty(Queue q) {
	return q->size == 0;
}

/**
 * Prints the queue to the given file with items space-separated
 */
void QueueDump(Queue q, FILE *fp) {
	for (Node curr = q->head; curr != NULL; curr = curr->next) {
		fprintf(fp, "%d ", curr->item);
	}
	fprintf(fp, "\n");
}

/**
 * Prints out information for debugging
 */
void QueueDebugPrint(Queue q) {

}

