
#include <stdlib.h>

#include "BSTree.h"
int Sum(BSTree t);
int BSTreeGetKth(BSTree t, int k) {	
	if(t == NULL) return 0;
	int NumLeft = Sum(t -> left);
	if(k < NumLeft) {
		return BSTreeGetKth(t -> left, k);
	}
	else if(k > NumLeft) {
		return BSTreeGetKth(t -> right, k - NumLeft - 1);
	} else {
		return t -> value;
	}
}

int Sum(BSTree t) {
	if(t == NULL) return 0;
	return 1 + Sum(t -> left) + Sum(t -> right);
}



