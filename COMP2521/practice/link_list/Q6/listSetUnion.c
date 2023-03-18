#include "list.h"
#include<stdbool.h>
Node copyS1(Node cur);
void do_Union(List Union, Node head);
bool check(Node cur, int val);
List listSetUnion(List s1, List s2) {
	List Union = newList();
	if(s1 -> head == NULL && s2 -> head == NULL) return Union;
	else if(s1 -> head == NULL && s2 -> head != NULL) {
		Union -> head = copyS1(s2 -> head);
		return Union;
	}
	else if(s1 -> head != NULL && s2 -> head == NULL) {
		Union -> head = copyS1(s1 -> head);
		return Union;
	}
	Union -> head = copyS1(s1 -> head);
	do_Union(Union, s2 -> head);
	return Union;
}

Node copyS1(Node cur) {
	if(cur == NULL) return cur;
	Node new = newNode(cur -> value);
	new -> next = copyS1(cur -> next);
	return new;
}

void do_Union(List Union, Node head) {
	Node cur = NULL;
	for(cur = Union -> head; cur -> next != NULL; cur = cur -> next);
	Node find = head;
	while(find != NULL) {
		if(check(Union -> head, find -> value)) {
			Node new = newNode(find -> value);
			cur -> next = new;
			cur = new;
		}
		find = find -> next;
	}
}

bool check(Node cur, int val) {
	if(cur == NULL) return true;
	if(cur -> value == val) return false;
	return check(cur -> next, val);
}
