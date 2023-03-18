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
void ShortestInsertEdge(Graph g, Graph shortest, int dest, int src, int *pred);
int findMinDis(double *dist, int nV, int *setV);
void relax(int v, Graph g, double *dist, int *pred);
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
// tracing Dijkstra's Algorithm.
Graph GraphMST(Graph g) {
    int nV = g -> nV; int dest = nV - 1;
    Graph shortest = GraphNew(nV);
    int *pred = malloc(nV * sizeof(int));
    for(int i = 0; i < nV; i++)
        pred[i] = -1;
    double *dist = malloc(nV * sizeof(double));
    for(int i = 0; i < nV; i++)
        dist[i] = 10000;
    int *setV = malloc(nV * sizeof(int));
    for(int i = 0; i < nV; i++)
        setV[i] = i;

    int numV = nV; int src = 0; dist[src] = 0;
    while (numV != 0) {
        int v = findMinDis(dist, nV, setV);
        setV[v] = -1;
        relax(v, g, dist, pred);
        numV--;
    }
    ShortestInsertEdge(g, shortest, dest, src, pred);
    free(dist); free(pred);
    return shortest;
}
void ShortestInsertEdge(Graph g, Graph shortest, int dest, int src, int *pred) {
    while (dest != src) {
        int v = dest; int w = pred[dest];
        if(pred[dest] == -1) {
            w = src;
        }
        double weight = g -> edges[v][w];
        if(weight <= 0) {
            return;
        }
        Edge e = {v, w, weight};
        GraphInsertEdge(shortest, e);
        dest = pred[dest];
    }
}
int findMinDis(double *dist, int nV, int *setV) {
    double min = 10000;
    int minindex = 0;
    for(int i = 0; i < nV; i++) {
        if(min > dist[i] && setV[i] != -1) {
            min = dist[i];
            minindex = i;
        }
    }
    return minindex;
}
void relax(int v, Graph g, double *dist, int *pred) {
    for(int w = 0; w < g -> nV; w++){
        if(GraphIsAdjacent(g, v, w)){
            double weight = g -> edges[v][w];
            if(dist[v] + weight < dist[w]) {
                dist[w] = dist[v] + weight;
                pred[w] = v;
            }
        }
    }
}
////////////////////////////////////////////////////////////////////////
static int validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

