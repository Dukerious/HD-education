// COMP2521 Assignment 2

// Written by: (z5306254) Sicong GAO
// Date: 9/11/2021

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "queue.h"
Queue RankFileInQueue(int argc, char **argv);
Queue MergeAllRankFile(Queue start);
void AddToQueue(char **rank, Queue Q);
void InitializeArray(int *a, int len);
void StoreMin(int *str, int *min, int len);
void pernutation(int *str, int *min, int len,
int start, int end, Queue merge, Queue head, double *dis);
void swap(int *a,int i,int j);
void printarray(int *a, int len);

int main(int argc, char *argv[]) {
    // store all content in rank files into queues,
    // per file per queue.
    Queue head = RankFileInQueue(argc, argv);

    // merge all pages in one queue, which is our C.
    Queue merge = MergeAllRankFile(head);
    //print_queue(head);
    //print_queue(merge);

    // set our array P, and we will store min_P into array min.
    int len = num_page(merge);
    int str[len];
    int min[len];
    // initial P arrange{1, 2, 3,...}.
    InitializeArray(str, len);
    // assume maximum distance.
    double dis = 100.0;
    // using brute-force search method, to try each possible.
    pernutation(str, min, len, 0, len, merge, head, &dis);

    // print the mini distance.
    printf("%.6lf\n", dis);
    //printarray(min, len);

    // print our sorted page list.
    PrintByArrayQueue(min, len, merge);
    freeQueue(head);
    freeQueue(merge);
    return 0;
}

// store all content in rank files into queues,
// per file per queue. and will return the first queue.
Queue RankFileInQueue(int argc, char **argv) {
    Queue prev = NULL;
    Queue start = NULL;
    // scan each rank file and add pages into some queues.
    for(int i = 1; i < argc; i++) {
        // rank is a tokensize, point to each page in a file.
        char **rank = find_collect_pages(argv[i]);
        // make a queue list, connect our each queue.
        if(prev == NULL) {
            start = queueAdd(prev, argv[i]);
            prev = start;
        }   else prev = queueAdd(prev, argv[i]);
        // after make a new queue, then add page in queue.
        AddToQueue(rank, prev);
        free_tokens(rank);
    }
    return start;
}

// add each page into queue, which in the same file.
void AddToQueue(char **rank, Queue Q) {
    for(int i = 0; rank[i] != NULL; i++) {
        // set pages to some vertex number.
        Page p = page_made(rank[i], 0, 0, 0, i + 1);
        QueueEnqueue(Q, p, SortingBystable);
    }
}

// merge all pages, which in diff rank files
// in one queue, which is our C.
Queue MergeAllRankFile(Queue start) {
    Queue merge = newQueue("p");
    mergeListQueue(start, merge);
    Markingindex(merge);
    return merge;
}

// Initialize a array.
void InitializeArray(int *a, int len) {
    for(int i = 0; i < len; i++) {
        a[i] = i + 1;
    }
}

// swap a[i] and a[j].
void swap(int *a,int i,int j){
    int c;
    c = a[i];
    a[i] = a[j];
    a[j] = c;
}

// if we find a distance less than previous one
// then undate our mini_p.
void StoreMin(int *str, int *min, int len) {
    for(int i = 0; i < len; i++) {
        min[i] = str[i];
    }
}

// our pernutation function, which will try each possible.
void pernutation(int *str, int *min, int len,
int start, int end, Queue merge, Queue head, double *dis){
    if(start<end){//end condition
        if(start+1 == end){
           double W = RankFileDis(head, merge, str);
           if(W < *dis) {
               StoreMin(str, min, len);
               *dis = W;
           }
            return;
        }else{
            int i;
            for(i=start;i<end;i++){
                swap(str,start,i); // swap
                pernutation(str, min, len, start+1,end, merge, head, dis);
                swap(str,start,i); //swap back
            }
        }
    }else{
        fprintf(stderr, "size invalid!\n");
        exit(1);
    }
}

// debugging function:
void printarray(int *a, int len) {
    for(int i = 0; i< len; i++) {
        printf("%d  ", a[i]);
    }
    putchar('\n');
}
