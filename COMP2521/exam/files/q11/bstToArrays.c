
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSTree.h"
void do_bstToArrays(BSTree t, char keys[], int left, int parents[], int leftSiblings[], int i);
int numnodes(BSTree t);
// Worst case time complexity of this solution: O(n)
void bstToArrays(BSTree t, char keys[], int parents[], int leftSiblings[]) {
	if(t == NULL) return;
	int i = numnodes(t -> left);
	do_bstToArrays(t, keys, -1, parents, leftSiblings, i);
}

void do_bstToArrays(BSTree t, char keys[], int left, int parents[], int leftSiblings[], int i) {
	if(t == NULL) return;
	keys[i] = t -> key;
	if(left == 1) {
		parents[i] = i - 1;
	} else if(left == 0){
		parents[i] = i - 2;
	} else if(left == -1){
		parents[i] = i;
	}
	leftSiblings[i] = i - 1;
	int num = numnodes(t -> left);
	do_bstToArrays(t -> left, keys, 1, parents, leftSiblings, i);
	do_bstToArrays(t -> right, keys, 0, parents, leftSiblings, i - num - 1);

}

int numnodes(BSTree t) {
	if(t == NULL) return 0;
	return 1 + numnodes(t -> left) + numnodes(t -> right);
}
