
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
void DepthSearch(Graph g, int nV, int *visited, int v);
void depthFirstSearch(Graph g, int src) {
	int nV = GraphNumVertices(g);
	int *visited = calloc(nV, sizeof(int));
	visited[src] = 1;
	DepthSearch(g, nV, visited, src);
	free(visited);
}

void DepthSearch(Graph g, int nV, int *visited, int v) {
	printf("%d ", v);
	for(int w = 0; w < nV; w++) {
		if(GraphIsAdjacent(g, v, w) && !visited[w]) {
			visited[w] = 1;
			DepthSearch(g, nV, visited, w);
		}
	}
}
