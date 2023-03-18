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
# define MAX_DIS 10000.00
Queue RankFileInQueue(int argc, char **argv);
Queue MergeAllRankFile(Queue start);
void AddToQueue(char **rank, Queue Q);
void InitializeArray(int *a, int len);
void StoreMin(int *str, int *min, int len);
void pernutation(int *str, int *min, int len,
int cid, Queue merge, Queue head, double *dis);
void swap(int *a,int i,int j);
void printarray(int *a, int len);

int main(int argc, char *argv[]) {
    // store all pages, which in rank files into queues,
    // per file per queue.
    Queue head = RankFileInQueue(argc, argv);

    // merge all pages in one queue, which is our list c.
    Queue merge = MergeAllRankFile(head);

    // set our array pern, which to store the current position. 
    //and we will use array min to store min_P (has minimum distance).
    int len = num_page(merge);
    int pern[len];
    int min[len];

    // initial P arrange{1, 2, 3,...}.
    InitializeArray(pern, len);

    // initial distance to maximum.
    double dis = MAX_DIS;
    // Use permutation to generate all possible P, but we won’t
    // check for impossible arrange list.
    pernutation(pern, min, len, 0, merge, head, &dis);

    // print the mini distance, if we met rank files, which are all empty,
    // just print 0.
    if(dis == MAX_DIS) {
        dis = 0;
    }
    printf("%.6lf\n", dis);

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
        // set pages to some order number.
        Page p = page_made(rank[i], 0, 0, 0, i + 1);
        QueueEnqueue(Q, p, SortingBystable);
    }
}

// merge all pages, which in some rank files
// in queue, which is our list C.
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
// then update our mini_p.
void StoreMin(int *str, int *min, int len) {
    for(int i = 0; i < len; i++) {
        min[i] = str[i];
    }
}


// Use permutation to generate all possible P, but we won’t
// check for impossible option arrange.
void pernutation(int *pern, int *min, int len,
int cid, Queue merge, Queue head, double *dis){
    // reach to end.
    if(cid == len - 1){
        double W = RankFileDis(head, merge, pern);
        if(W < *dis) {
            // update the new minimum position.
            StoreMin(pern, min, len);
            *dis = W;
        }
        return;
    }else{
        int i;
        for(i = cid; i < len; i++){
            swap(pern, cid, i); // swap
            // if we check the sum of weight(until cid) 
            //is larger than our current min_dis, then
            // we can stop check other impossible position under this cid, 
            // and go back to parent literation.
            // in this way, we can save the time complexity,
            // and avoid to calculate the impossible position.
            if(CheckWeight(head, merge, pern, dis, cid))
                pernutation(pern, min, len, cid + 1, merge, head, dis);
            swap(pern, cid, i); //swap back
        }
    }
}


// debugging function:
void printarray(int *a, int len) {
    for(int i = 0; i< len; i++) {
        printf("%d  ", a[i]);
    }
    putchar('\n');
}
