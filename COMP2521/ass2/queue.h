#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#define MAX 4096
// set the page struct.
// store some datas.
typedef struct page {
    char name[MAX];
    double PR;
    int outdegree;
    int vertex;
    int searching;
}Page;

typedef struct queue *Queue;
typedef struct node *Node;

Queue newQueue(char *Qname);

void freeQueue(Queue Q);

bool QueueIsEmpty(Queue Q);

int num_page(Queue Q);

// Enqueue in a certain order.
void QueueEnqueue(Queue Q, Page p, bool (*fp) (Page, Page));

// create a page by page's data.
Page page_made(char *name, double PR, 
int outdegree, int searching, int vertex);

// some sorting condition functions:
// whcih will use in QueueEqueue.
bool SortingBystable(Page a, Page b);

bool SortingByPR(Page a, Page b);

bool SortingBySearch(Page a, Page b);

bool SortingByVertex(Page a, Page b);

bool SortingByalpha(Page a, Page b);


// dequeue the first item.
Page QueueDequeue(Queue Q);

// find a page vertex by page name.
int find_vertex(Queue Q, char *p);

// find the page's data in a queue by name.
Page find_pageInQueue(Queue Q, char *p);

// find the diff between the new and old PageRank.
// and store the new one.
double diff(Queue Q, double new_PR, char *p);

// updating the outdegree for a page.
void outdegree_updating(Queue Q, int v);

// find the outdegree for a page.
int found_outdegree(Queue Q, int vertex);

// find the PageRank for a page.
double PR_finding(Queue Q, int vertex);

// print PageRank to a file in desending order.
void StorePRInFile(Queue Q);

// part2, sorting our queue by alpha, PR and search,
// step by step.
void SortingSearchQueue(Queue Q);

// make a new queue, and connect to the previous one.
Queue queueAdd(Queue prev, char *Qname);

// find a queue by search item.
Queue find_queue(Queue Q, char *item);

// initialize all pages index in a queue.
void Markingindex(Queue Q);

// Combine all the queues in the queue list into one queue.
void mergeListQueue(Queue Q, Queue merge);

// merge two queues, avoid duplicate page.
void merge_queue(Queue Q, Queue merge);

// adding searchRank for a page.
void AddSearchRank(Queue Q, char *name);

// print a queue follow the order in array.
void PrintByArrayQueue(int *a, int len, Queue Q);

// calculate scaled-footrule distance. 
double RankFileDis(Queue Q, Queue P, int *PR);

// check the weight for some pages until cid index.
bool CheckWeight(Queue Q, Queue P, int *PR, 
double *dis, int cid);

//some debugging functions:

void print_queue_name(Queue Q);

void print_Byvertex(Queue Q, int vertex);

void print_queue(Queue Q);

#endif