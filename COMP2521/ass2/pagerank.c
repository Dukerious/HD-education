// COMP2521 Assignment 2

// Written by: Sicong GAO
// Date: 3/11/2021

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utility.h"
static void store_page_in_graph(Graph g, char **pages, int page_num);
static void connect_pages(Graph g, char **pages);
static void store_page_outlink(Graph g, char *p);
static double all_pageRank(char **pages, Graph g, double d, int page_num);
static double PageRank(Graph g, char *Pi, double d, int page_num);
void modify_FileName(char name[MAX], char *p, char *add);

int main(int argc, char *argv[]) {
    // convert arguments to number d, diffPR and maxIterations.
    double d = atof(argv[1]);
    double diffPR = atof(argv[2]);
    int maxIterations = atoi(argv[3]);

    // find each page in collection file.
    char **pages = find_collect_pages("collection.txt");
    assert(pages != NULL);

    // setting our graph structure.
    int page_num = num_pages(pages);
    Graph g = GraphNew(page_num);

    // insert each page into a graph.
    store_page_in_graph(g, pages, page_num);
    // setting the relation of connect for each page(outlink and inlink).
    connect_pages(g, pages);

    // PageRank algorithm
    double diff = diffPR;
    int interation = 0;
    // until we has a stable pagerank.
    while (interation < maxIterations && diff >= diffPR) {
        diff = all_pageRank(pages, g, d, page_num);
        interation++;
    }
    // store rank to oagerankList.txt and free all pages and graph.
    print_graph_queue(g);
    free_tokens(pages);
    GraphFree(g);
    return 0;
}


// store each page into graph.
static void store_page_in_graph(Graph g, char **pages, int page_num) {
    for(int vertex = 0; pages[vertex] != NULL; vertex++) {
        // initialize each page, PageRank = 1 / N at start, and outlink is zero.
        Page p = page_made(pages[vertex], 
    (double) 1/ page_num, 0, 0, vertex);
        store_page(g, p, SortingByPR);
    }
}

// find outlinks for each page, and set the connection.
static void connect_pages(Graph g, char **pages) {
    for(int i = 0; pages[i] != NULL; i++) {
        store_page_outlink(g, pages[i]);
    }
}

// store a page outlink into graph.
static void store_page_outlink(Graph g, char *p) {
    // find the page text file.
    char name[MAX];
    modify_FileName(name, p, ".txt");
    // for each outlink, set the edges.
    //eg: page a -> b, set directed edge ab.
    char **outlink = FindOutlink(name);
    for(int i = 0; outlink[i] != NULL; i++) {
        char file[MAX];
        modify_FileName(file, outlink[i], ".txt");
        insert_edge(g, p, outlink[i]);
    }
    free_tokens(outlink); 
}

// modifiy the page name for fopen.
void modify_FileName(char name[MAX], char *p, char *add) {
    strcpy(name, p);
    strcat(name, add);
}

// store new PageRank for each pages, and return the sum_diff.
static double all_pageRank(char **pages, Graph g, double d, int page_num) {
    double sum = 0;
    for(int i = 0; pages[i] != NULL; i++) {
        double new_PR = PageRank(g, pages[i], d, page_num);
        sum += find_diff(g, pages[i], new_PR);
    }
    return sum;
}

// calculate a PageRank by formula.
static double PageRank(Graph g, char *Pi, double d, int page_num) {
    return (double) (((1 - d)/page_num) + (d * cal_PR(g, Pi)));
}






