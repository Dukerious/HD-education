
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

int dayTrip(Graph g, Vertex s, Vertex vs[]) {
    // TODO
    // use the vs array as a visited array first
    // check standard roads
    for (int i = 0; i < g->nV; i++) {
        if (g->stdEdges[s][i]) {
            vs[i] = 1;
        }
    }

    // check fast roads
    for (int i = 0; i < g->nV; i++) {
        if (g->fastEdges[s][i]) {
            vs[i] = 1;
            for (int j = 0; j < g->nV; j++) {
                if (g->fastEdges[i][j] && j != s) {
                    vs[j] = 1;
                }
            }
        }
    }

    // now convert the visited array to an array of vertices
    int numReachable = 0;
    for (int i = 0; i < g->nV; i++) {
        if (vs[i] == 1) {
            vs[numReachable++] = i;
        }
    }
    return numReachable;
}

