#include<stdio.h>

void BSTreeTraversal(BSTree t) {
    if (t == NULL) 
        return;

    printf("%d", t -> data);
    BSTreeTraversal(t -> left);
    BSTreeTraversal(t -> right);

}