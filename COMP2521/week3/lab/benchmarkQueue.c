
#include <stdio.h>
#include <stdlib.h>

#include "Queue.h"

int main(void) {
	Queue q = QueueNew();
	for (Item it = 0; it < 20000; it++) {
		QueueEnqueue(q, it);
	}
	for (int i = 0; i <= 10000; i++) {
		QueueDequeue(q);
	}
	QueueFree(q);
}

