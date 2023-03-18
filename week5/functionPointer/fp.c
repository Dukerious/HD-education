#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
int process(int a, int b, int(*fp)(int, int));
int f1 (int a, int b);
int f2 ();
int main(int argc, char **argv) {
    int a = 10;
    int b = 20;
    printf("%d\n", process(a, b, f1));
    printf("%d\n", process(a, b, f2));
}

int process(int a, int b, int(*fp)(int, int)) {
    return fp(a, b);
}

int f1 (int a, int b) {
    return a + b;
}

int f2 (int a, int b) {
    return a - b;
}