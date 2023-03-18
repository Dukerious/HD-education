
#include <stdlib.h>

#include "tree.h"
int Sum(Tree t);
bool TreeIsPerfectlyBalanced(Tree t) {
	if(t == NULL) return true;
	if(abs(Sum(t -> left) - Sum(t -> right)) > 1) 
		return false;
	bool check = TreeIsPerfectlyBalanced(t -> left);
	if(!check) return false;
	return TreeIsPerfectlyBalanced(t ->right);
}

int Sum(Tree t) {
	if(t == NULL) return 0;
	return 1 + Sum(t -> left) + Sum(t -> right);
}