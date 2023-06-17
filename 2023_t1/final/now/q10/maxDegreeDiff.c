
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
static void FillDegree(AdjList l, int v, int *indegreev, int *outdegreev);
static int FindMax(int *indegreev, int *outdegreev, int nv);
//O(n + m)
int maxDegreeDiff(Graph g) {
    // O(1)
    int nv = GraphNumVertices(g);

    //O(n)
    int *indegreev = calloc(nv, sizeof(int));
     //O(n)
    int *outdegreev = calloc(nv, sizeof(int));

    //O(n + m)
    for (int v = 0; v < nv; v++) {
        FillDegree(g -> edges[v], v, indegreev, outdegreev);
    }


    //O(n)
    int max = FindMax(indegreev, outdegreev, nv);
    free(indegreev);
    free(outdegreev);
    return max;
}
static void FillDegree(AdjList l, int v, int *indegreev, int *outdegreev) {
    if(l == NULL) return;
    int w = l -> v;
    indegreev[w] ++;
    outdegreev[v] ++;
    FillDegree(l -> next, v, indegreev, outdegreev);
}

static int FindMax(int *indegreev, int *outdegreev, int nv) {
    int max = -1;
    for (int v = 0; v < nv; v++) {
        int degreediff = abs(indegreev[v] - outdegreev[v]);
        if(max < degreediff) max = degreediff;
    }
    return max;
}
