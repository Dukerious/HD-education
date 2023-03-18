
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Queue.h"
void do_breadthFirstSearch(Graph g, int nV, int src, int *visited);
void breadthFirstSearch(Graph g, int src) {
	int nV = GraphNumVertices(g);
	int *visited = calloc(nV, sizeof(int));
	do_breadthFirstSearch(g, nV, src, visited);
	free(visited);
}

void do_breadthFirstSearch(Graph g, int nV, int src, int *visited) {
	Queue q = QueueNew();
	visited[src] = 1;
	QueueEnqueue(q, src);
	while(!QueueIsEmpty(q)) {
		int v = QueueDequeue(q);
		printf("%d ", v);
		for(int w = 0; w < nV; w++) {
			if(GraphIsAdjacent(g, v, w) && !visited[w]) {
				QueueEnqueue(q, w);
				visited[w] = 1;
			}
		}
	}
	QueueFree(q);
}

