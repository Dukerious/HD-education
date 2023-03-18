
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Queue.h"
int count_dis(int *visited, int dest, int src);
int do_shortestDistance(Graph g, int src, int dest, int nV, int *visited);
int shortestDistance(Graph g, int src, int dest) {
	if(src == dest) {
		return 0;
	}
	int nV = GraphNumVertices(g);
	int *visited = malloc(nV * sizeof(int));
	for(int i = 0; i < nV; i++)
		visited[i] = -1;
	int dis = do_shortestDistance(g, src, dest, nV, visited);
	free(visited);
	return dis;
}	

int do_shortestDistance(Graph g, int src, int dest, int nV, int *visited) {
	Queue q = QueueNew();
	QueueEnqueue(q, src);
	visited[src] = src;
	while (!QueueIsEmpty(q)) {
		int v = QueueDequeue(q);
		for(int w = 0; w < nV; w++) {
			if(GraphIsAdjacent(g, v, w) && visited[w] == -1) { 
				visited[w] = v;
				QueueEnqueue(q, w);
				if(w == dest) {
					QueueFree(q);
					return count_dis(visited, dest, src);
				}
			}
		}
	}
	QueueFree(q);
	return -1;
}

int count_dis(int *visited, int dest, int src) {
	int dis = 0;
	while (dest != src) {
		dest = visited[dest];
		dis++;
	}
	return dis;
}