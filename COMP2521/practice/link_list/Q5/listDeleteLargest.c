#include "list.h"
int findmax(Node cur, int max);
Node deletelargest(Node cur, int largest);

int listDeleteLargest(List l) {
    int max = l -> head -> value;
    int largest = findmax(l -> head, max);
    l -> head = deletelargest(l -> head, largest);
    return largest;
}

int findmax(Node cur, int max) {
    if(cur == NULL) return max;
    if(cur -> value > max) max = cur -> value;
    return findmax(cur -> next, max);
}

Node deletelargest(Node cur, int largest) {
    if(cur == NULL) return cur;
    if(cur -> value == largest) {
        Node next = cur -> next;
        free(cur);
        return next;
    }
    cur -> next = deletelargest(cur -> next, largest);
    return cur;
} 