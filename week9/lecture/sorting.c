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

bool less(int a, int b);
void swap(int a[], int m, int n);
/*void shellSort(int a[], int lo, int hi);
void bubble_sort(int *a, int lo, int hi);
void selection_sort(int *a, int lo, int hi);
void swap(int *a, int *b);
bool less(int a, int b);
void medianOfThree(Item a[], int lo, int hi);
void quicksort(int a[], int lo, int hi);
int partition(int a[], int lo, int hi);
void mergesort(Item a[], int lo, int hi);
void merge(Item a[], int lo, int mid, int hi);*/
int main(void) {
    int array[] = { 18, 14, 17, 19, 8, 13, 6, 8, 18, 5, 20, 13 };
    //selection_sort(array, 0, 8);
    quick_sort(array, 0, 11);
    //bubble_sort(array, 0, 11);
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











/*void insertion_sort(int *a, int lo, int hi) {
    for(int i = lo + 1; i < hi + 1; i++) {
        int val = a[i];
        int j;
        for(j = i; j > lo; j--) {
            if(val > a[j - 1])
                break;
            a[j] = a[j - 1];
        }
        a[j] = val;
    }
}
void bubble_sort(int *a, int lo, int hi) {
    for(int i = hi; i >= lo; i--) {
        int sorted = 1;
        for(int j = lo; j < i; j++) {
            if(a[j] > a[j + 1]) {
                swap(&a[j], &a[j + 1]);
                sorted = 0;
            }
        }
        if(sorted == 1) {
            break;
        }
    }
}
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void selection_sort(int *a, int lo, int hi) {
    for(int i = lo; i < hi; i++) {
        for(int j = i + 1; j <= hi; j++) {
            if(a[i] > a[j]) {
                swap(&a[i], &a[j]);
            }
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
      for (i = start+1; i <= hi; i++) {
         val = a[i];
         for (j = i; j >= start; j -= h) {
            if (val >= a[j-h]) break;
            a[j] = a[j-h];
         }
         a[j] = val;
      }
   }
}
void medianOfThree(Item a[], int lo, int hi) {
    int mid = (lo+hi)/2;
    if (less(a[mid],a[lo])) swap(&a[lo], &a[mid]);
    if (less(a[hi],a[mid])) swap(&a[mid], &a[hi]);
    if (less(a[mid],a[lo])) swap(&a[lo], &a[mid]);
    // now, we have a[lo] < a[mid] < a[hi]
    // swap a[mid] to a[lo+1] to use as pivot
    swap(&a[mid], &a[lo+1]);
}
void quicksort(int a[], int lo, int hi)
{
   int i; // index of pivot
   if (hi <= lo) return;
   //medianOfThree(a, lo, hi);
   i = partition(a, lo, hi);
   quicksort(a, lo, i-1);
   quicksort(a, i+1, hi);
}

int partition(int a[], int lo, int hi)
{
   int v = a[lo];  // pivot
   int  i = lo+1, j = hi;
   for (;;) {
      while (less(a[i],v) && i < j) i++;
      while (less(v,a[j]) && j > i) j--;
      if (i == j) break;
      swap(&a[i],&a[j]);
   }
   j = less(a[i],v) ? i : i-1;
   swap(&a[lo],&a[j]);
   return j;
}


void mergesort(Item a[], int lo, int hi)
{
    int mid = (lo + hi) / 2; // mid point
    if (hi <= lo) return;
    mergesort(a, lo, mid);
    mergesort(a, mid+1, hi);
    merge(a, lo, mid, hi);
}

void merge(Item a[], int lo, int mid, int hi)
{
    int i, j, k, nitems = hi - lo + 1;
    Item *tmp = malloc(nitems * sizeof(Item));
    i = lo; j = mid + 1; k = 0;
    // scan both segments, copying to tmp
    while (i <= mid && j <= hi) {
        if (less(a[i], a[j]))
        tmp[k++] = a[i++];
        else
        tmp[k++] = a[j++];
    }
    // copy items from unfinished segment
    while (i <= mid) tmp[k++] = a[i++];
    while (j <= hi) tmp[k++] = a[j++];
    //copy tmp back to main array
    for (i = lo, k = 0; i <= hi; i++, k++)
        a[i] = tmp[k];
    free(tmp); 
}




bool less(int a, int b) {
    return(a < b);
}
void print_array(int *array, int hi) {
    for (int i = 0; i < hi; i++) {
        printf("%d ", array[i]);
    }
    putchar('\n');
}*/