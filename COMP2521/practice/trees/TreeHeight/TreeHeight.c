
#include <stdlib.h>

#include "tree.h"

int TreeHeight(Tree t) {
    if(t == NULL) return -1;
    int left = TreeHeight(t -> left);
    int right = TreeHeight(t -> right);
    return 1 + (left < right ? right : left);
}

