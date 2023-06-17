#include "queue.h"
//初始化
void QueueInit(Queue* pq)
{
	assert(pq);					 //判断指针的有效性
	pq->_head = pq->_tail = NULL;//队头和队尾指向空指针
}
//销毁
void QueueDestory(Queue* pq)
{
	assert(pq);
	QueueNode* cur = pq->_head;
	while (cur)
	{
		QueueNode* next = cur->_next;
		free(cur);
		cur = next;
	}
	pq->_head = pq->_tail = NULL;
}
//入队
void QueuePush(Queue* pq, QDataType x)
{
	assert(pq);
	QueueNode* newnode = (QueueNode*)malloc(sizeof(QueueNode));//为新节点申请内存空间
	if (newnode == NULL)//判断内存空间是否申请成功
	{
		printf("内存不足!\n");
		exit(-1);
	}
	newnode->_data = x;   //新节点储存数据
	newnode->_next = NULL;//新节点的下一个指向NULL，即新节点作为队尾
	if (pq->_head == NULL)//将新节点入队
	{
		pq->_head = pq->_tail = newnode;
	}
	else
	{
		pq->_tail->_next = newnode;
		pq->_tail = newnode;
	}

    printf("push for data %d\n\n\n", x);
}
//出队
QDataType QueuePop(Queue* pq)
{
	assert(pq);
	assert(pq->_head);
    QDataType retval = pq -> _head -> _data;
	QueueNode* next = pq->_head->_next;
	free(pq->_head);
	pq->_head = next;
	if (pq->_head == NULL)
	{
		pq->_tail = NULL;
	}
    printf("pop for data %d\n\n\n", retval);
    return retval;
}
//访问队首的元素
QDataType QueueFront(Queue* pq)
{
	assert(pq);
	assert(pq->_head);
	return pq->_head->_data;
}
//访问队尾的元素
QDataType QueueBack(Queue* pq)
{
	assert(pq);
	assert(pq->_tail);
	return pq->_tail->_data;
}
//返回1是空，返回0是非空
int QueueEmpty(Queue* pq)
{
	assert(pq);
	return pq->_head == NULL ? 1 : 0;
}
//获取数据的个数
int QueueSize(Queue* pq)
{
	assert(pq);
	QueueNode* cur = pq->_head;
	int size = 0;
	while (cur)
	{
		++size;
		cur = cur->_next;
	}
	return size;
}
