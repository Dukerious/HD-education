#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
typedef int Item;
typedef struct HeapRep {
    Item *items; // array of Items
    int nitems; // #items in array
    int nslots; // #elements in array
} HeapRep;
typedef HeapRep *Heap;
Heap newHeap(int N);
void heap_insert(Heap h, Item a);
void fixup(int *a, int hi);
Item heap_delete(Heap h);
void fixdown(int *a, int hi, int i);
void swap(int *a, int i, int j);

void heap_sort(int *a, int lo, int hi);
int insert(int *a, int item, int num);
int delete (int *a, int num);
void heap_sort_new(int *a, int lo, int hi);
void print_array(int *array, int hi);
int main () {
    int a[] = {3, 2, 7, 3, 7, 6, 3, 6, 2};
    heap_sort(a, 0, 8);
    print_array(a, 8);
    return 0;
}
void print_array(int *array, int hi) {
    for (int i = 0; i <= hi; i++) {
        printf("%d ", array[i]);
    }
    putchar('\n');
}

// first of all, making heap, 
// should sort revere.
// N / 2(the last parent) to 1 (the first parent).
void heap_sort_new(int *a, int lo, int hi) {
    int N = hi - lo + 1;
    int *temp = a + lo - 1;
    // making a heap.
    // since the last parent iteration is N / 2.
    // down to up, making heap.
    for(int i = N / 2; i > 0; i--) {
        fixdown(temp, N, i);
    }

    while(N > 1) {
        swap(temp, 1, N);
        N--;
        fixdown(temp, N, 1);
    }

}
void heap_sort(int *a, int lo, int hi) {
    int nitems = hi - lo + 1;
    int *temp = malloc((nitems+1) * sizeof(int));
    int num = 0;
    for(int i = lo; i <= hi; i++) {
        insert(temp, a[i], num);
        num ++;
    }
    for(int i = lo; i <= hi; i++) {
        a[i] = delete(temp, num);
        num--;
    }
}
int insert(int *a, int item, int num) {
    num++;
    a[num] = item;
    fixup(a, num);
    return num;
}
int delete (int *a, int num) {
    int top = a[1];
    a[1] = a[num];
    num--;
    fixdown(a, num, 1);
    return top;
}
Heap newHeap(int N)
{
    Heap new = malloc(sizeof(HeapRep));
    Item *a = malloc((N+1)*sizeof(Item));
    assert(new != NULL && a != NULL);
    new->items = a; // no initialisation needed
    new->nitems = 0; // counter and index
    new->nslots = N; // index range 1..N
    return new;
}
void heap_insert(Heap h, Item a) {
    h -> nitems++;
    h -> items[h -> nitems] = a;
    fixup(h -> items, h -> nitems);
}
void fixup(int *a, int hi) {
    int i = hi;
    while (i > 1) {
        int j = i;
        i = i/2;
        if(a[i] > a[j]) swap(a, i, j);
    }
}
Item heap_delete(Heap h) {
    Item top = h -> items[1];
    h -> items[1] = h -> items[h -> nitems];
    h -> nitems--;
    fixdown(h -> items, h -> nitems, 1);
    return top;
}
void fixdown(int *a, int hi, int i) {
    while (i < hi) {
        int j = 2 * i;
        if(j > hi) break;
        if(j + 1 <= hi && a[j] > a[j+1]) j++;
        if(a[i] < a[j]) break;
        swap(a, i, j);
        i = j;
    }
}
void swap(int *a, int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp;
}