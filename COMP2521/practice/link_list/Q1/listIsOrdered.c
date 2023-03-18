
#include "list.h"
bool check_descending(Node cur, int prev);
bool check_ascending(Node cur, int prev);
bool listIsOrdered(List l) {
	if(l == NULL || l -> head == NULL) return true;
	Node head = l -> head;
	bool check1 = check_ascending(head -> next , head -> value);
	bool check2 = check_descending(head -> next, head -> value);
	if(!check1 && !check2) return false;
	return true;
}
bool check_descending(Node cur, int prev) {
	if(cur == NULL) return true;
	if(cur -> value > prev) return false;
	return check_descending(cur -> next, cur -> value);
}
bool check_ascending(Node cur, int prev) {
	if(cur == NULL) return true;
	if(cur -> value < prev) return false;
	return check_ascending(cur -> next, cur -> value);
}