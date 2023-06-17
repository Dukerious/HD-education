#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
void swap(int *a, int i, int j);
void bubble_sort(int lo, int hi, int *a);
void insertion_sort(int *a, int lo, int hi);
void NewInsertion_sort(int *a, int lo, int hi);
void selection_sort(int *a, int lo, int hi);

void quick_sort(int *a, int lo, int hi);
int partition(int *a, int lo, int hi);
void findmedian(int *a, int lo, int hi);

void merge_sort(int *a, int lo, int hi);
void merge(int *a, int lo, int hi);
void PrintArray(int *a, int lo, int hi);
int main(void) {
    int a[] = {5, 3, 2, 1, 9, 6, 5, 8};
    PrintArray(a, 0, 7);
    //bubble_sort(0, 7, a);
    //insertion_sort(a, 0, 7);
    //NewInsertion_sort(a, 0, 7);
    //selection_sort(a, 0, 7);
    //quick_sort(a, 0, 7);
    merge_sort(a, 0, 7);
    PrintArray(a, 0, 7);
    return 0;
}

void bubble_sort(int lo, int hi, int *a) {
    for(int i = lo; i <= hi; i++) {
        bool sorted = true;
        for(int j = hi; j > i; j--) {
            if(a[j] > a[j-1]) {
                swap(a, j, j-1);
                sorted = false;
            }
        }
        if(sorted) break;
    }
}

void insertion_sort(int *a, int lo, int hi) {
    for(int i = lo; i < hi; i++) {
        int val = a[i+1];
        int j;
        for (j = i + 1; j > lo; j--) {
            if(val <= a[j-1]) break;
            a[j] = a[j-1];
        }
        a[j] = val;
    }
}
void NewInsertion_sort(int *a, int lo, int hi) {
    for(int i = lo; i < hi; i++) {
        for(int j = i + 1; j > lo; j--) {
            if(a[j] <= a[j-1]) break;
            swap(a, j, j-1);
        }
    }
}
void selection_sort(int *a, int lo, int hi) {
    for(int i = lo; i < hi; i++) {
        int min = i;
        for(int j = i + 1; j <= hi; j++) {
            if(a[j] > a[min]) min = j;
        }
        swap(a, i, min);
    }
}
void quick_sort(int *a, int lo, int hi) {
    if(lo >= hi) return;
    findmedian(a, lo, hi);
    int i = partition(a, lo, hi);
    quick_sort(a, lo, i - 1);
    quick_sort(a, i + 1, hi);
}
int partition(int *a, int lo, int hi) {
    int val = a[lo];
    int i = lo + 1; int j = hi;
    for(;;) {
        while(i < j && a[i] > val) i++;
        while(i < j && a[j] < val) j--;
        if(i == j) break;
        swap(a, i, j);
    }
    i = a[i] > val ? i : i - 1;
    swap(a, i, lo);
    return i; 
}
void findmedian(int *a, int lo, int hi) {
    int mid = (lo + hi) / 2;
    if(a[lo] < a[mid]) swap(a, lo, mid);
    if(a[mid] < a[hi]) swap(a, mid, hi);
    if(a[lo] < a[mid]) swap(a, lo, mid);
    swap(a, mid, lo);
}
void merge_sort(int *a, int lo, int hi) {
    if(lo >= hi) return;
    int mid = (lo + hi) / 2;
    merge_sort(a, lo, mid);
    merge_sort(a, mid + 1, hi);
    merge(a, lo, hi);
}
void merge(int *a, int lo, int hi) {
    int size = hi - lo + 1;
    int *temp = calloc(size, sizeof(int));
    int i = lo; int mid = (lo + hi) / 2; int j = mid + 1;
    int index = 0;
    while(i <= mid && j <= hi) {
        if(a[i] < a[j]) 
            temp[index++] = a[j++];
        else temp[index++] = a[i++];
    }
    while(i <= mid) temp[index++] = a[i++];
    while(j <= hi) temp[index++] = a[j++];
    index = 0; int m = lo;
    while (m <= hi) a[m++] = temp[index++];
    free(temp);
}
void swap(int *a, int i, int j) {
    int temp = a[i];
    a[i] = a[j];
    a[j] = temp; 
}

void PrintArray(int *a, int lo, int hi) {
    for(int i = lo; i <= hi; i++) {
        printf("%d ", a[i]);
    }
    putchar('\n');
}