#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
typedef int Item;
void print_array(int *array, int hi);
void insertion_sort(int *a, int lo, int hi);
void bubble_sort(int a[], int lo, int hi);
void selection_sort(int *a, int lo, int hi);

void merge_sort(int a[], int lo, int hi);
void do_merge_sort(int a[], int lo, int mid, int hi);

void quick_sort(int a[], int lo, int hi);
void find_median(int a[], int lo, int hi);
int partition(int a[], int lo, int hi);

void shellSort(int a[], int lo, int hi);


bool less(int a, int b);
void swap(int a[], int m, int n);

int main(void) {
    int array[] = { 18, 14, 17, 19, 8, 13, 6, 8, 18, 5, 20, 13 };

    merge_sort(array, 0, 11);

    print_array(array, 12);
    return 0;
}

void insertion_sort(int a[], int lo, int hi) {
    for(int i = lo + 1; i <= hi; i++) {
        for (int j = i; j > lo; j--) {
            if(!less(a[j], a[j-1])) {
                break;
            }
            swap(a, j, j-1);
        }
    }
}

void shellSort(int a[], int lo, int hi)
{
   int hvals[8] = {701, 301, 132, 57, 23, 10, 4, 1};
   int g, h, start, i, j, val;
   for (g = 0; g < 8; g++) {
      h = hvals[g];
      start = lo + h;
      for (i = start; i <= hi; i++) {
         val = a[i];
         for (j = i; j >= start; j -= h) {
            if (val >= a[j-h]) break;
            a[j] = a[j-h];
         }
         a[j] = val;
      }
   }
}

void bubble_sort(int a[], int lo, int hi) {
    for(int i = hi; i > lo; i--) {
        bool sorted = true;
        for(int j = lo; j < i; j++) {
            if(!less(a[j], a[j+1])) {
                swap(a, j, j+1);
                sorted = false;
            }
        }
        if(sorted) {
            break;
        }
    }
}

void selection_sort(int a[], int lo, int hi) {
    for(int i = lo; i < hi; i++) {
        for(int j = i + 1; j <= hi; j++) {
            if(!less(a[i], a[j])) {
                swap(a, i, j);
            }
        }
    }
}

void merge_sort(int a[], int lo, int hi) {
    if(lo >= hi) {
        return;
    }
    int mid = (lo + hi) / 2;
    merge_sort(a, lo, mid);
    merge_sort(a, mid + 1, hi);
    do_merge_sort(a, lo, mid, hi);
}
void do_merge_sort(int a[], int lo, int mid, int hi) {
    int *temp = calloc(hi - lo + 1, sizeof(int));
    int index = 0;
    int i = lo;
    int j = mid + 1;
    while (i <=mid && j <= hi) {
        if(less(a[i], a[j])) {
            temp[index++] = a[i++];
        } else {
            temp[index++] = a[j++];
        }
    }
    while(i <= mid) temp[index++] = a[i++];
    while(j <= hi) temp[index++] = a[j++];
    index = 0; int m = lo;
    while (m <= hi) a[m++] = temp[index++];
    free(temp);
}

void quick_sort(int a[], int lo, int hi) {
    find_median(a, lo, hi);
    if(lo >= hi) {
        return;
    }
    int i = partition(a, lo, hi);
    quick_sort(a, lo, i - 1);
    quick_sort(a, i + 1, hi);
}
void find_median(int a[], int lo, int hi) {
    int mid = (lo + hi) / 2;
    if(!less(a[lo], a[mid])) swap(a, lo, mid);
    if(!less(a[mid], a[hi])) swap(a, mid, hi);
    if(!less(a[lo], a[mid])) swap(a, lo, mid);
    swap(a, lo, mid);
}

int partition(int a[], int lo, int hi) {
    int val = a[lo];
    int i = lo + 1; int j = hi;
    for(;;) {
        while(i < j && !less(val, a[i])) i++;
        while(i < j && !less(a[j], val)) j--;
        if(i == j) break; 
        swap(a, i, j);
    }
    i = (val < a[i]) ? i - 1 : i;
    swap(a, i, lo);
    return i; 
}

bool less(int a, int b) {
    return (a < b);
}

void swap(int a[], int m, int n) {
    int temp = a[m];
    a[m] = a[n];
    a[n] = temp;
}
void print_array(int *array, int hi) {
    for (int i = 0; i < hi; i++) {
        printf("%d ", array[i]);
    }
    putchar('\n');
}

