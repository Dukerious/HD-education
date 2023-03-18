// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "PQ.h"

struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent
};

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited);
static int  validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////
Edge **CopyEdge(Graph g);
bool UsedEmpty(int *usedV, int len);

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

////////////////////////////////////////////////////////////////////////

int GraphNumVertices(Graph g) {
    return g->nV;
}

bool GraphInsertEdge(Graph g, Edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight > 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        g->edges[e.w][e.v] = e.weight;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != 0.0) {   // edge e in graph
        g->edges[v][w] = 0.0;
        g->edges[w][v] = 0.0;
        g->nE--;
        return true;
    } else {
        return false;
    }
}

double GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            if (g->edges[v][w] != 0.0) {
                printf("Edge %d - %d: %lf\n", v, w, g->edges[v][w]);
            }
        }
    }
}

bool GraphHasCycle(Graph g) {
    bool *visited = calloc(g->nV, sizeof(bool));
    assert(visited != NULL); // lazy error checking
    
    for (int v = 0; v < g->nV; v++) {
        if (!visited[v] && doHasCycle(g, v, v, visited)) {
            free(visited);
            return true;
        }
    }

    free(visited);
    return false;
}

static bool doHasCycle(Graph g, Vertex v, Vertex prev, bool *visited) {
    visited[v] = true;
    for (int w = 0; w < g->nV; w++) {
        if (g->edges[v][w] != 0.0) {
            if (!visited[w]) {
                if (doHasCycle(g, w, v, visited)) {
                    return true;
                }
            } else if (w != prev) {
                return true;
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////
// Your task

Graph GraphMST(Graph g) {
    Graph MST = GraphNew(g -> nV);
    Edge **UnusedEdge = CopyEdge(g);
    int *usedV = calloc(g -> nV ,sizeof(int));
    usedV[0] = 1;
    while(!UsedEmpty(usedV, g -> nV)) {
        double MinWeight = 100000;
        Edge *MinEdge = NULL;
        int NumEdge = g -> nE;
        for(int i = 0; i < NumEdge; i++) {
            Edge min = PQExtract(pq);
            while(usedV[min.v] && !usedV[min.w]) {
                PQInsert(pq, min);
                min = PQExtract(pq);
            }
            if(usedV[min.v] && !usedV[min.w]) {
                if(MinWeight > UnusedEdge[i] -> weight) {
                    MinWeight = UnusedEdge[i] -> weight;
                    Edge *MinTemp = UnusedEdge[i];
                    UnusedEdge[i] = MinEdge;
                    MinEdge = MinTemp;
                }
            }
        }
        Edge min = {MinEdge -> v, MinEdge -> w, MinEdge -> weight};
        GraphInsertEdge(MST, min);
        usedV[min.w] = 1;
    }
    return MST;
}
Edge **CopyEdge(Graph g) {
    Edge **UnusedEdge = calloc(g -> nE + 1, sizeof(Edge*));
    int e;
    for(e = 0; e < g -> nE; e++) {
        UnusedEdge[e] = calloc(1, sizeof(Edge));
    }
    UnusedEdge[e] = NULL;
    int index = 0;
    for(int v = 0; v < g -> nV; v++) {
        for(int w = v + 1; w < g -> nV; w++) {
            if(GraphIsAdjacent(g, v, w)) {
                UnusedEdge[index] -> v = v;
                UnusedEdge[index] -> w = w;
                UnusedEdge[index] -> weight = g -> edges[v][w];
                index++;
            }
        }
    }
    return UnusedEdge;
}
bool UsedEmpty(int *usedV, int len) {
    for(int i = 0; i < len; i++) {
        if(!usedV[i]) {
            return false;
        }
    }
    return true;
}
////////////////////////////////////////////////////////////////////////

static int validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

