
#include "list.h"
int countdup(Node cur, int val);
int numDupesInOrderedList(List l) {
	int mindel = 0;
	Node cur = l -> head;
	int prev = -100;
	while(cur != NULL) {
		if(prev != cur -> value) {
		mindel += countdup(l -> head, cur -> value) - 1;
		}
		prev = cur -> value;
		cur = cur -> next;
	}
	return mindel;
}
int countdup(Node cur, int val) {
	if(cur == NULL) return 0;
	return countdup(cur -> next, val) + ((cur -> value == val) ? 1 : 0);
}

