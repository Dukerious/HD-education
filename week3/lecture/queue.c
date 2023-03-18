struct queue {
   Dot head;
   Dot tail;
   int length;
}
struct dot {
   Tree root;
   Dot next;
};

void do_freequeue(Dot cur);
Queue QueueNew() {
    Queue new = malloc(sizeof(*new));
    if(new == NULL) {
        fprintf(stderr, "malloc space error!\n");
    }
    new -> head = NULL;
    new -> tail = NULL;
}
Dot DotNew(Tree root) {
    Dot new = malloc(sizeof(*new));
    if(new == NULL) {
        fprintf(stderr, "malloc space error!\n");
    }
    new -> root = root;
    new -> next = NULL;
}
void QueueEnqueue(Queue Q, Tree root) {
    if(Q -> head == NULL) {
        Q -> head = root;
        Q -> tail = root;
    }
    else {
        Dot new = DotNew(root);
        Dot old = Q -> tail;
        old -> next = new;
        Q -> tail = new; 
    }
}
Tree QueueDequeue(Queue Q) {
    Tree found = Q -> head -> root; 
    Q -> head = Q -> head -> next;
    return found;
}
void freequeue(Q) {
    do_freequeue(Q -> head);
    free(Q);
}
void do_freequeue(Dot cur) {
    if(cur == NULL) {
        return;
    }
    do_freequeue(cur -> next);
    free(cur);
}
