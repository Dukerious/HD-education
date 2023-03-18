
#include "list.h"
int delete(Node cur, int maximum);
int listDeleteLargest(List l) {
	Node cur = l -> head;
	int maximum = cur -> value; 
	return delete(cur, maximum);
}


int delete(Node cur, int maximum) {
	if (cur -> next == NULL) {
		if (maximum < cur -> value) {
			maximum = cur -> value;
		}
		return maximum;
	}
	if (maximum < cur -> value) {
		maximum = cur -> value;
	}
	maximum = delete(cur -> next, maximum);
	if (cur -> next -> value == maximum) {
		Node f = cur -> next;
		cur -> next = f -> next;
		free(f);
	}
	return maximum;
}

