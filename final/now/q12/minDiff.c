
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"
static void grabsamelevelnode(BSTree t, int l, int now, int *level, int *len);
int minDiff(BSTree t, int l) {
    int len = 0;
    int *level = malloc(100 * sizeof(int));
    grabsamelevelnode(t, l, 0, level, &len);
    if(len <= 1) return 0;

    int mindiff = 100000;
    for(int i = 0; i < len - 1; i++) {
        int diff = abs(level[i] - level[i+1]);
        if(mindiff > diff) mindiff = diff;
    }
    return mindiff;
}

static void grabsamelevelnode(BSTree t, int l, int now, int *level, int *len) {
    if(t == NULL || now > l) return;
    if(l == now) {
        level[(*len) ++] = t -> key;
    }

    now += 1;
    grabsamelevelnode(t -> left, l, now, level, len);
    grabsamelevelnode(t -> right, l, now, level, len);
}

