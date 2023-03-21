
#include "tree.h"
Tree NewNode(int val);
Tree TreeCopy(Tree t, int depth) {
    if(t == NULL || depth < 0) return NULL;
    Tree new = NewNode(t -> value);

    new -> left = TreeCopy(t->left, depth - 1);
    new -> right = TreeCopy(t -> right, depth - 1);

    return new;
}
Tree TreeNode(int val) {
    Tree new = malloc(sizeof(*new));
    if(new == NULL) {
        fprintf(stderr, "Error memory, stack overflow!\n");
    }
    new -> value val;
    new -> left = new -> right = NULL;
    return new;
}