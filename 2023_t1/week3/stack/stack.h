#include<stdio.h>
#include<stdlib.h>
#include <assert.h>

typedef int STDataType;
typedef struct Stack
{
	STDataType* _a;
	int _top;//栈顶下标
	int _capacity;//栈的容量
}Stack;

//初始化和销毁
void StackInit(Stack* pst);
void StackDestory(Stack* pst);

//入栈
void StackPush(Stack*pst,STDataType x);

//出栈
void StackPop(Stack* pst);

//获取数据的个数
int StackSize(Stack* pst);

//返回1是空，返回0是非空
int StackEmpty(Stack* pst);

//获取栈顶的数据
STDataType StackTop(Stack* pst);