#include<stdio.h>

int main(void) {

    int a;
    printf("hello, world!\n");
    int num = 0;
    after = add(num);
    return 0;
}

int add(int num) {
    if(num == 10) 
        return num;
    
    num = num + 1;
    return add(num);
}