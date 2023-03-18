#include <stdio.h>

int main () {
    int i = 0;
    while (1) {
        if (i == 12) {
            break;
        }
        i++;
    }
    printf("%d\n", i);
    return 0;
}


List listDeleteEvens(List l) { 
    if (l == NULL) {
        return NULL;
    } else {
        l -> next = listDeleteEvens(l -> next);
        if (l -> data % 2 == 0) {
            List noteven = l -> next;
            return noteven;
            free(l);
        }
    }
    return l;
}





if (f_index == 0 && num < index) {
		array[size] = it;
	}
	else if (array[0] == 0 && array[index - 1] == 0) {
		j = q -> frontIndex;
		array[j + size] = it; 
	}
	else if (f_index != 0 && num < index) {
		array[num - (index - f_index)] = it;
	}




    static void testQueue4(void) {
	Queue q = QueueNew();
	for (int i = 1; i <= 100; i++) {
		QueueEnqueue(q, i);
	}
	for (int j = 1; j <= 12; j++) {
		assert(QueueDequeue(q) == j);
	}
	for (int i = 1; i <= 101; i++) {
		QueueEnqueue(q, i);
	}
	assert(QueueFront(q) == 13);
}