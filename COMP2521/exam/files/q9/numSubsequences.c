
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"
int Alternatives(Node curA, Node firstB, int tolerance);
int count(Node cur1, Node cur2);
int countlistsize(Node cur);
// Worst case time complexity of this solution: O((n-m)m)
int numSubsequences(List listA, List listB, int tolerance) {
	return Alternatives(listA -> first, listB -> first, tolerance);
}

int Alternatives(Node curA, Node firstB, int tolerance) {
	int check = 0;
	if(curA == NULL || countlistsize(firstB) > countlistsize(curA)) return 0;
	if(count(curA, firstB) <= tolerance) check = 1;
	return check + Alternatives(curA -> next, firstB, tolerance);
}
int count(Node cur1, Node cur2) {
	if(cur2 == NULL) return 0;
	return ((cur1 -> value == cur2 -> value) ? 0 : 1) + count(cur1->next, cur2->next);
}

int countlistsize(Node cur) {
	if(cur == NULL) return 0;
	return 1 + countlistsize(cur -> next);
}
