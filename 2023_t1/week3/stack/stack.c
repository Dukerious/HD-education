#include "stack.h"

//初始化和销毁
void StackInit(Stack* pst)
{
	assert(pst);//判断指针的有效性
	pst->_a =malloc(sizeof(STDataType) * 4);
	pst->_top = 0;
	pst->_capacity = 4;
}
void StackDestory(Stack* pst)
{
	assert(pst);//判断指针的有效性
	free(pst->_a);
	pst->_a = NULL;
	pst->_top = pst->_capacity = 0;
}
//入栈
void StackPush(Stack* pst, STDataType x)
{
	assert(pst);//判断指针的有效性
	//空间不够则增容
	if (pst->_top == pst->_capacity)
	{
		pst->_capacity *= 2;//每次增容空间的大小为上一次的两倍
		STDataType* tmp = (STDataType*)realloc(pst->_a, sizeof(STDataType) * pst->_capacity);
		if (tmp == NULL)//判断是否内存申请成功
		{
			printf("内存不足!\n");
			exit(-1);
		}
		else
		{
			pst->_a = tmp;
		}
	}
	pst->_a[pst->_top] = x;
	pst->_top++;
    printf("push data %d\n", x);
}
//出栈
void StackPop(Stack* pst)
{
	assert(pst);//判断指针的有效性
	assert(pst->_top > 0);
    printf("pop data %d\n", pst->_a[pst->_top - 1]);
	--pst->_top;
}
//获取数据的个数
int StackSize(Stack* pst)
{
	assert(pst);//判断指针的有效性
	return pst->_top;
}
//返回1是空，返回0是非空
int StackEmpty(Stack* pst)
{
	assert(pst);//判断指针的有效性
	return pst->_top == 0 ? 1 : 0;
	//return !pst->_top;
}
//获取栈顶的数据
STDataType StackTop(Stack* pst)
{
	assert(pst);//判断指针的有效性
	assert(pst->_top > 0);
	return pst->_a[pst->_top - 1];
}
