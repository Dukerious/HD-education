
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
static double *GraphDijkstra(Graph g, Vertex src);
static int findMinDis(double *dist, int nV, int *setV);
static void relax(int v, Graph g, double *dist, int *pred);
static double grabWeight(Graph g, int v, int w);

int dayTrip(Graph g, Vertex s, Vertex vs[]) {
    double *dist = GraphDijkstra(g, s);
    int index = 0;
    for (int i = 0; i < g -> nV; i++) {
        printf("%lf\n", dist[i]);
        if(dist[i] <= 1.0 && dist[i] > 0) {
            vs[index++] = i;
        }
    }

    free(dist);
    return index;
}





static double *GraphDijkstra(Graph g, Vertex src) {
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

    dist[src] = 0;
    for (int i = 0; i < nV; i++) {
        int v = findMinDis(dist, nV, setV);
        setV[v] = -1;
        relax(v, g, dist, pred);
    }

    free(setV); free(pred);
    return dist;
}


static int findMinDis(double *dist, int nV, int *setV) {
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
static void relax(int v, Graph g, double *dist, int *pred) {
    for(int w = 0; w < g -> nV; w++){
        double weight = grabWeight(g, v, w);
        if (weight == -1) continue;
        if(dist[v] + weight < dist[w]) {
            dist[w] = dist[v] + weight;
            pred[w] = v;
        }
    }
}

static double grabWeight(Graph g, int v, int w) {
    if(g -> fastEdges[v][w]) return 0.5;
    if(g -> stdEdges[v][w]) return 1.0;
    return -1.0;
}