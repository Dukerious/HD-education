#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
void swap(int *a,int i,int j);
void printarray(int *a, int len);
void pernutation(int *a, int len, int cid);
int main(void) {
    int a[] = {1, 2, 3};
    pernutation(a, 3, 0);
    return 0;
}

void pernutation(int *a, int len, int cid) {
    if(cid == len - 1) {
        //printarray(a, len);
        return;
    }
    for (int i = cid; i < len; i++) {
        swap(a, i, cid);
        printf("the cid is %d\n", cid);
        printarray(a, len);
        pernutation(a, len, cid + 1);
        swap(a, i, cid);
    }
}

void swap(int *a,int i,int j){
    int c;
    c = a[i];
    a[i] = a[j];
    a[j] = c;
}
void printarray(int *a, int len) {
    for(int i = 0; i< len; i++) {
        printf("%d  ", a[i]);
    }
    putchar('\n');
}