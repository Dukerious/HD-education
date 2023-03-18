
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
void do_numReachable(Graph g, int nV, int src, int *visited);
int count_reach(int nV, int *visited);
int numReachable(Graph g, int src) {
	int nV = GraphNumVertices(g);
	int *visited = calloc(nV, sizeof(int));
	visited[src] = 1;
	do_numReachable(g, nV, src, visited);
	int num = count_reach(nV, visited);
	free(visited);
	return num;
}

void do_numReachable(Graph g, int nV, int src, int *visited) {
	for(int v = 0; v < nV; v++) {
		if(GraphIsAdjacent(g, src, v) && !visited[v]) {
			visited[v] = 1;
			do_numReachable(g, nV, v, visited);
		}
	}
}
int count_reach(int nV, int *visited) {
	int count = 0;
	for(int i = 0; i < nV; i++) {
		if(visited[i]) {
			count++;
		}
	}
	return count;
}

