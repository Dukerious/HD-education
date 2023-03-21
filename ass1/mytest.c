#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "bBST.h"
static void printTree(Node root);
int main(int argc, char **argv) {
    Tree t = TreeNew();
    TreeInsert(t, 10);
    TreeInsert(t, 15);
    printTree(t->root);
    printf("%d\n", TreeSearch(t, 2));

    TreeDelete(t, 15);
    printTree(t->root);
    return 0;
}

static void printTree(Node root) {
    if(root == NULL) return;
    printf("%d\n", root -> key);
    // printf("and the hight is %d\n", root->height);
    printTree(root -> left);
    printTree(root -> right);
}