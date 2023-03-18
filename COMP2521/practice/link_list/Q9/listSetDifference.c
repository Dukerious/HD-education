#include "list.h"
bool CheckItems(Node cur, int val);
List listSetDifference(List l1, List l2) {
	List diff = newList();
	Node cur = l1 -> head;
	Node add = diff -> head;
	while(cur != NULL) {
		if(CheckItems(l2 -> head, cur -> value)) {
			Node new = newNode(cur -> value);
			if(add == NULL) diff -> head = new;
			else add -> next = new;
			add = new;
		}
		cur = cur -> next;
	}
	return diff;
}

bool CheckItems(Node cur, int val) {
	if(cur == NULL) return true;
	if(cur -> value == val) return false;
	return CheckItems(cur -> next, val);
}
