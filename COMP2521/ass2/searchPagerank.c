// COMP2521 Assignment 2

// Written by: (z5306254) Sicong GAO
// Date: 7/11/2021

#include <assert.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"
#include "utility.h"
Queue FilePageInqueue(char *file);
Queue search_items(char *file, Queue Q);
Queue SearchRank(char **argv, Queue Q);

int main(int argc, char *argv[]) {
    // store the pagerank in a queue.
    Queue Q = FilePageInqueue("pagerankList.txt");
    Queue start = search_items("invertedIndex.txt", Q);
    Queue merge = SearchRank(argv, start);

    // print our Pages
    // with the same number of matching search terms.
    // ordered in descending order by their Weighted PageRank.
    SortingSearchQueue(merge);

    freeQueue(Q);
    freeQueue(start);
    freeQueue(merge);
    return 0;
}

// store the pagerank in a queue.
Queue FilePageInqueue(char *file) {
	FILE *f = fopen(file, "rb");
	if(f == NULL) {
        fprintf(stderr, "invalid file!\n");
        exit(EXIT_FAILURE);
    }
    // count the num of char in this file.
    long size = file_size(f);
    Queue new = newQueue(NULL);
    // read pageranklist file line by line,
    // and store pages to queue.
    for (int index = 0; ; index++) {
        char **page = tokenizeFile(f, ", ", size);
        // until after the last line, we stop to read. 
        if(page[0] == NULL) {
            free_tokens(page);
            break;
        }
        // set the page data{name, PR....}.
        Page p = page_made(page[0], atof(page[2]), 
        atoi(page[1]), 0, index);
        free_tokens(page);
        QueueEnqueue(new, p, SortingBystable); 
    }
    fclose(f);
    return new;
}

// using some queues to store search items and 
// pages which contain that search items.
// and the queue name is a search item.
Queue search_items(char *file, Queue Q) {
    // open a file.
    FILE *f = fopen(file, "rb");
	if(f == NULL) {
        fprintf(stderr, "invalid file\n");
        return NULL;
    }
    // find the num of char, which this file contains.
    long size = file_size(f);
    Queue prev = NULL;
    Queue start = NULL;
    // until we made all search item queues.
    for(;;) {
        char **search = tokenizeFile(f, " ", size);
        // we meet the end of the file.
        if(search[0] == NULL){
            free_tokens(search);
            break;
        }
        // link our queues.
        if(prev == NULL) {
            start = queueAdd(prev, search[0]);
            prev = start;
        }   else prev = queueAdd(prev, search[0]);

        // for each queue, enqueue their pages.
        for(int i = 1; search[i] != NULL; i++) {
            Page p = find_pageInQueue(Q, search[i]);
            // if page doesn't exist in invertedindex file.
            if(strcmp(p.name, "") == 0) 
                p = page_made(search[i], 0, 0, 0, 0);
            QueueEnqueue(prev, p, SortingBystable);
        }
        free_tokens(search);
    }
    fclose(f);
    return start;
}

// merge all relevant pages, which contain our target search items,
//avoid duplicate.
Queue SearchRank(char **argv, Queue Q) {
    Queue merge = newQueue(NULL);
    for (int i = 1; argv[i] != NULL; i++) {
        Queue found = find_queue(Q, argv[i]);
        // if its a valid search item, then merge.
        if(found != NULL) {
            merge_queue(found, merge);   
        }
    }
    return merge;
}

