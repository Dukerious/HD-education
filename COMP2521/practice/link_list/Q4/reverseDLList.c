
#include "list.h"
Node do_reverseDLList(List l, Node cur);
void reverseDLList(List l) {
	if(l == NULL || l -> first == NULL) return;
	do_reverseDLList(l, l -> first) -> next = NULL;
}

Node do_reverseDLList(List l, Node cur) {
	if(cur == l -> first) l -> last = cur;
	if(cur -> next == NULL) {
		l -> first = cur;
		cur -> prev = NULL;
		return cur;
	}
	Node temp = do_reverseDLList(l, cur -> next);
	temp -> next = cur;
	cur -> prev = temp;
	return cur;
}

