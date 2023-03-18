
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"
BSTree CreateNode();
BSTree BSTreeInsert(BSTree t, int val) {
	if(t == NULL) {
		return CreateNode(val);
	}
	if(t -> value < val) {
		t -> right = BSTreeInsert(t -> right, val);
	}
	else if(t -> value > val) {
		t -> left = BSTreeInsert(t -> left, val);
	}
	return t;
}

BSTree CreateNode(int val) {
	BSTree new = malloc(sizeof(*new));
	if(new == NULL) {
		fprintf(stderr, "error memory!\n");
	}
	new -> value = val;
	new -> left = new -> right = NULL;
	return new;
}

