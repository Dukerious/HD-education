#include "stack.h"
void TestStack();
void PrintStack(STDataType* data, int len);

int main()
{
	TestStack();
	return 0;
}

void TestStack()
{
	//后进先出是相对入的时候在栈里面的数据
	Stack st;
	StackInit(&st);
    PrintStack(st._a, st._top);

	StackPush(&st, 1);
    PrintStack(st._a, st._top);

	StackPush(&st, 2);
    PrintStack(st._a, st._top);

	StackPush(&st, 3);
    PrintStack(st._a, st._top);

	StackPush(&st, 4);
    PrintStack(st._a, st._top);

	while (!StackEmpty(&st))
	{
		StackPop(&st);
        PrintStack(st._a, st._top);
	}
	StackDestory(&st);
}

void PrintStack(STDataType* data, int len) {
    printf("=======\n");
    for (int i = len - 1; i >= 0; i--) {
        printf("   %d\n", data[i]);
    }
    printf("=======\n\n\n");
}
