
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Queue.h"
int FindFurthest(Graph g, int nV, int src);
int countdis(int *visited, int src, int dest);
void BreadthSearch(Graph g, int nV, int *visited, int src);
int furthestReachable(Graph g, int src) {
	int nV = GraphNumVertices(g);
	return FindFurthest(g, nV, src);
}


int FindFurthest(Graph g, int nV, int src) {
	int *visited = malloc(nV * sizeof(int));
	for(int i = 0; i < nV; i++) 
		visited[i] = -1;
	BreadthSearch(g, nV, visited, src);
	if(countdis(visited, src, src) == -1) {
		free(visited); return src;
	}
	int vertex = -1; int max = -1;
	for(int v = 0; v < nV; v++) {
		int dis = countdis(visited, src, v);
		if(dis != -1) {
			if(dis > max) {
				vertex = v; max = dis;
			}
			else if(dis == max) {
				if(vertex < v) vertex = v;
			}
		}
	}
	free(visited); return vertex;
}
int countdis(int *visited, int src, int dest) {
	int dis = 0;
	if(visited[dest] == -1) return -1;
	while(dest != src) {
		dest = visited[dest];
		dis++;
	}
	return dis;
}
void BreadthSearch(Graph g, int nV, int *visited, int src) {
	visited[src] = src;
	Queue new = QueueNew();
	QueueEnqueue(new, src);
	while(!QueueIsEmpty(new)) {
		int v = QueueDequeue(new);
		for(int w = 0; w < nV; w++) {
			if(visited[w] == -1 && GraphIsAdjacent(g, v, w)) {
				visited[w] = v; QueueEnqueue(new, w);
			}
		}
	}
	QueueFree(new);
}
