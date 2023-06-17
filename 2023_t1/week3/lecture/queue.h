#include<stdio.h>
#include<assert.h>
#include<stdlib.h>
#include "BSTree.h"
typedef struct dot *Dot;
typedef struct queue *Queue;

Queue QueueNew();

Dot DotNew(Tree root);

void QueueEnqueue(Queue Q, Tree root);

Tree QueueDequeue(Queue Q);

void freequeue(Q);

