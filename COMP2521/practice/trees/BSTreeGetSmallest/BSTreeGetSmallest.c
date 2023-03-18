
#include "BSTree.h"

#include <stdlib.h>
#include <stdio.h>
BSTree BSTreeGetSmallest(BSTree t) {
	if(t == NULL) return t;
	if(t -> left == NULL) return t;
	return BSTreeGetSmallest(t -> left);
}

