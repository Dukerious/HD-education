// Implementation of the Undirected Graph ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"

struct graph {
	int nV;
	double **edges;
};
static int *GrabPath(int src, int minindex, int *pred, int nV);
static int *GrabPathTask3(int src, Graph g, int minindex, int *pred, int nV, int *energy);
static int findMinDis(double *dist, int nV, int *setV);
static void relax(int v, Graph g, double *dist, int *pred);
static void relaxTask3(int v, Graph g, double *dist, int *pred, int* energy, int energyfull);
static bool validVertex(Graph g, Vertex v);
static Graph doGraphNew(int nV);

//O(V^2)
Graph GraphNew(int nV) {
	assert(nV > 0);
	
	Graph g = doGraphNew(nV);
	for (Vertex v = 0; v < nV; v++) {
		for (Vertex w = 0; w < nV; w++) {
			g->edges[v][w] = -1;
		}
	}
	
	return g;
}


static Graph doGraphNew(int nV) {
	Graph g = malloc(sizeof(*g));
	if (g == NULL) {
		fprintf(stderr, "GraphNew: Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
	
	g->nV = nV;
	
	g->edges = malloc(nV * sizeof(double *));
	if (g->edges == NULL) {
		fprintf(stderr, "GraphNew: Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
	for (Vertex v = 0; v < nV; v++) {
		g->edges[v] = malloc(nV * sizeof(double));
		if (g->edges[v] == NULL) {
			fprintf(stderr, "GraphNew: Insufficient memory!\n");
			exit(EXIT_FAILURE);
		}
	}
	
	return g;
}

////////////////////////////////////////////////////////////////////////
// Destructors

void GraphFree(Graph g) {
	assert(g != NULL);
	
	for (Vertex v = 0; v < g->nV; v++) {
		free(g->edges[v]);
	}
	free(g->edges);
	free(g);
}

////////////////////////////////////////////////////////////////////////
// General Graph Functions

int GraphNumVertices(Graph g) {
	assert(g != NULL);
	
	return g->nV;
}

void GraphAddEdge(Graph g, Vertex v, Vertex w, double weight) {
	assert(g != NULL);
	assert(validVertex(g, v));
	assert(validVertex(g, w));
	assert(v != w);
	
	g->edges[v][w] = weight;
}

void GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
	assert(g != NULL);
	assert(validVertex(g, v));
	assert(validVertex(g, w));
	assert(v != w);
	
	g->edges[v][w] = -1;
}

bool GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
	assert(g != NULL);
	assert(validVertex(g, v));
	assert(validVertex(g, w));
	
	if(g->edges[v][w] != -1) return true;
    return false;
}


int *GraphDijkstra(Graph g, int src, int energysrc, int energyfull, double *dis, int *top, int topnum, bool task3) {
    
    int nV = g -> nV;
    int *pred = malloc(nV * sizeof(int));
    for(int i = 0; i < nV; i++)
        pred[i] = -1;
    double *dist = malloc(nV * sizeof(double));
    for(int i = 0; i < nV; i++)
        dist[i] = 10000;
    int *setV = malloc(nV * sizeof(int));
    for(int i = 0; i < nV; i++)
        setV[i] = i;

    int *energy = NULL;
    if(task3) {
        energy = calloc(nV, sizeof(int));
        energy[src] = energyfull - energysrc;
        if(energy[src] < 0) {
            int *path = malloc(sizeof(int));
            path[0] = -2;
            return path;
        }
    }

    dist[src] = 0; pred[src] = src;
    for (int i = 0; i < nV; i++) {
        int v = findMinDis(dist, nV, setV);
        setV[v] = -1;
        (task3) ? relaxTask3(v, g, dist, pred, energy, energyfull) : relax(v, g, dist, pred);
    }
    
    *dis = -1;
    int minindex = 0;
    for (int i = 0; i < topnum; i++) {
        int end = top[i];
        if(dist[end] < *dis || *dis == -1) {
            *dis = dist[end];
            minindex = end;
        }
    }


    free(setV);
    free(dist);

    int *path = NULL;
    path = (task3) ? GrabPathTask3(src, g, minindex, pred, nV, energy) : GrabPath(src, minindex, pred, nV);
    free(pred);

    if(task3) {
        free(energy);
    }

    return path;
}

static int *GrabPath(int src, int minindex, int *pred, int nV) {

    int *reversed = malloc(nV * sizeof(int));
    int next = minindex;
    int len = 0;
    while(next != src) {
        reversed[len] = next;
        if(next == -1) {
            len++;
            break;
        }
        next = pred[next];
        len++;
    }

    reversed[len] = src;

    int *path = malloc((len + 2) * sizeof(int));
    int index = 0;
    for (int i = len; i >= 0; i--) {
        path[index] = reversed[i];
        index++;
    }

    path[index] = -2;
    free(reversed);

    return path;
}


static int *GrabPathTask3(int src, Graph g, int minindex, int *pred, int nV, int *energy) {

    int *reversed = malloc(2 * nV * sizeof(int));
    int next = minindex;
    int len = 0;
    while(next != src) {
        reversed[len] = next;

        if(next == -1) {
            len++;
            break;
        }
        next = pred[next];
        len++;
    }

    reversed[len] = src;


    int *path = malloc(2 * (len + 2) * sizeof(int));
    int index = 0;
    for (int i = len; i >= 0; i--) {
        path[index] = reversed[i];
        if(i - 1 >= 0 && reversed[i - 1] >= 0 && reversed[i] >= 0 && GraphIsAdjacent(g, reversed[i], reversed[i - 1])) {
            if(energy[path[index]] < g ->edges[reversed[i]][reversed[i - 1]]) {
                index++;
                path[index] = reversed[i];
            }
        }

        index++;
    }

    path[index] = -2;
    free(reversed);
    return path;
}

static int findMinDis(double *dist, int nV, int *setV) {
    double min = -1;
    int minindex = -1;
    for(int i = 0; i < nV; i++) {
        if((min == -1 || min > dist[i]) && setV[i] != -1) {
            min = dist[i];
            minindex = i;
        }
    }
    return minindex;
}
static void relax(int v, Graph g, double *dist, int *pred) {
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

static void relaxTask3(int v, Graph g, double *dist, int *pred, int* energy, int energyfull) {
    for(int w = 0; w < g -> nV; w++){
        if(GraphIsAdjacent(g, v, w)){
            double weight = g -> edges[v][w];
            bool needrest = false;
            if(energy[v] < weight) needrest = true;
            if(energyfull < weight) continue;

            if((needrest && dist[v] + 2 < dist[w])) {
                dist[w] = dist[v] + 2;
                pred[w] = v;
                energy[w] = energyfull - weight;
            } 
            else if ((!needrest && dist[v] + 1 < dist[w])) {
                dist[w] = dist[v] + 1;
                pred[w] = v;
                energy[w] = energy[v] - weight;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////

static bool validVertex(Graph g, Vertex v) {
	return (v >= 0 && v < g->nV);
}

