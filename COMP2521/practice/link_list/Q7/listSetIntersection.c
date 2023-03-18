#include "list.h"
bool CheckSame(Node cur, int val);
List listSetIntersection(List s1, List s2) {
	List intsec = newList();
	Node cur = s1 -> head;
	Node add = intsec -> head;
	while(cur != NULL) {
		if(CheckSame(s2 -> head, cur -> value)) {
			Node new = newNode(cur -> value);
			if(add == NULL) intsec -> head = new;
			else add -> next = new;
			add = new;
		}
		cur = cur -> next;
	}
	return intsec;
}

bool CheckSame(Node cur, int val) {
	if(cur == NULL) return false;
	if(cur -> value == val) return true;
	return CheckSame(cur -> next, val);
}