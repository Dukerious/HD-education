
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Queue.h"
int CountNum(Graph g, int nV, int src, int dist);
void bfs(Graph g, int nV, int src, int *visited, int *dis);
int StoreDis(int *visited, int nV, int src, int dest);
int numWithin(Graph g, int src, int dist) {
	int nV = GraphNumVertices(g);
	return CountNum(g, nV, src, dist);
}


int CountNum(Graph g, int nV, int src, int dist) {
	int *visited = malloc(nV * sizeof(int));
	int *dis = malloc(nV * sizeof(int));
	for(int i = 0; i < nV; i++) {
		visited[i] = -1; dis[i] = -1;
	}
	bfs(g, nV, src, visited, dis);
	int count = 0;
	for(int v = 0; v < nV; v++) {
		if(dis[v] != -1) {
			if(dis[v] <= dist) count++;
		}
	}
	free(visited); free(dis);
	return count;
}
void bfs(Graph g, int nV, int src, int *visited, int *dis) {
	Queue q = QueueNew();
	visited[src] = src;
	QueueEnqueue(q, src);
	while(!QueueIsEmpty(q)) {
		int v = QueueDequeue(q);
		for(int w = 0; w < nV; w++) {
			if(GraphIsAdjacent(g, v, w) && visited[w] == -1) {
				visited[w] = v;
				QueueEnqueue(q, w);
			}
		}
	}
	for(int i = 0; i < nV; i++) {
		dis[i] = StoreDis(visited, nV, src, i);
	}
	QueueFree(q);
}

int StoreDis(int *visited, int nV, int src, int dest) {
	int count = 0;
	while(dest != src) {
		if(visited[dest] == -1) return -1;
		dest = visited[dest];
		count++;
	}
	return count;
}
