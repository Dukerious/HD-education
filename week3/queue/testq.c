#include "queue.h"
void TestQueue();
void PrintQueue(Queue* pq);

int main()
{
	TestQueue();
	return 0;
}


void TestQueue()
{
	Queue q;
	QueueInit(&q);
    PrintQueue(&q);
	QueuePush(&q, 1);
    PrintQueue(&q);

	QueuePush(&q, 2);
    PrintQueue(&q);

	QueuePush(&q, 3);
    PrintQueue(&q);

	QueuePush(&q, 4);
    PrintQueue(&q);

	while (!QueueEmpty(&q))
	{
        QueuePop(&q);
        PrintQueue(&q);
	}
	QueueDestory(&q);
}

void PrintQueue(Queue* pq) {
    printf("[");
    QueueNode *cur = pq -> _head;
    while(cur) {
        if (cur == pq -> _tail) {
            printf("%d", cur -> _data);
        }
        else printf("%d, ", cur -> _data);
        cur = cur -> _next;
    }
    printf("]\n");
}
