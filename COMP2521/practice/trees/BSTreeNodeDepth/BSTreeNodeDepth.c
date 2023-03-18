
#include <stdlib.h>

#include "BSTree.h"
int BSTreeNodeDepth(BSTree t, int key) {
	if (t == NULL) return -1;
	if(t -> value < key) {
		int val = BSTreeNodeDepth(t -> right, key);
		return (val == -1) ? -1 : 1 + val;
	}
	if(t -> value > key) {
		int val = BSTreeNodeDepth(t -> left, key);
		return (val == -1) ? -1 : 1 + val;
	}
	return 0;
}

