
#include "list.h"
Node doReverse(List l, Node cur);
void listReverse(List l) {
	if(l == NULL || l -> head == NULL) return;
	doReverse(l, l -> head) -> next = NULL;
}
Node doReverse(List l, Node cur) {
	if(cur -> next == NULL) {
		l -> head = cur;
		return cur;
	}
	doReverse(l, cur -> next) -> next = cur;
	return cur;
}

