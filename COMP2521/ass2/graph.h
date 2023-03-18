#ifndef GRAPH_H
#define GRAPH_H
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
typedef struct graph *Graph;
typedef struct edge {
    int v;
    int w;
}Edge;

Graph GraphNew(int nV);

void GraphFree(Graph g);

// page1 outlink to page2, setting a edge and insert.
void insert_edge(Graph g, char *p1, char *p2);

// store pages to page queue in graph.
void store_page(Graph g, Page p, bool (*fp) (Page, Page));

// calculate a part of PageRank,
// d*....
double cal_PR(Graph g, char *Pi);

// find different between new and old PageRank. 
double find_diff(Graph g, char *p, double new_PR);

// print PageRank in descending order.
void print_graph_queue(Graph g);

// print each edges in graph, just for debugging.
void print_graph(Graph g);
#endif