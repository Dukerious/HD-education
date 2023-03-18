
#include "list.h"

bool listIsPalindromic(List l) {
	Node last = l -> last;
	Node first = l -> first;
	while (last != NULL && first != NULL) {
		if(first -> value != last -> value) return false;
		first = first -> next;
		last = last -> prev;
	}
	return true;
}