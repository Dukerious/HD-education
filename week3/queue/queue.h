#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

typedef int QDataType;

typedef struct QueueNode
{
	QDataType _data;		//用来存储数据
	struct QueueNode* _next;//用来指向下一个结构体
}QueueNode;

typedef struct Queue
{
	QueueNode* _head; //存放整个队列的队头
	QueueNode* _tail; //存放整个队列的队尾
}Queue;

//初始化
void QueueInit(Queue* pq);

//销毁
void QueueDestory(Queue* pq);

//入队
void QueuePush(Queue* pq, QDataType x);

//出队
QDataType QueuePop(Queue* pq);

//访问队首的元素
QDataType QueueFront(Queue* pq);

//访问对尾的元素
QDataType QueueBack(Queue* pq);

//返回1是空，返回0是非空
int QueueEmpty(Queue* pq);

//获取数据的个数
int QueueSize(Queue* pq);

