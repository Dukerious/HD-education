
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
void do_numReachable(Graph g, int v, int nV, bool *visited);
int count_Recahable(int nV, bool *visited);
int numReachable(Graph g, int src) {
	int nV = GraphNumVertices(g);
	bool *visited = calloc(nV, sizeof(bool));
	visited[src] = true;
	do_numReachable(g, src, nV, visited);
	int num = count_Recahable(nV, visited);
	free(visited);
	return num;
}

void do_numReachable(Graph g, int v, int nV, bool *visited) {
	for(int w = 0; w < nV; w++) {
		if(GraphIsAdjacent(g, v, w) && !visited[w]) {
			visited[w] = true; 
			do_numReachable(g, w, nV, visited);
		}
	}
}
int count_Recahable(int nV, bool *visited) {
	int count = 0;
	for(int i = 0; i < nV; i++) {
		if(visited[i]) count++;
	}
	return count;
}

