
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
int checkInOut(Graph g, int v);
void setfollowers(Graph g, int src, int followers[]);
bool dfsPathCheck(Graph g, int nV, Vertex v, Vertex dest);
bool do_dfsPathCheck(Graph g, int nV, Vertex v, Vertex dest, int *visited);
// Worst case time complexity of this solution: O(n^2)
void findPopularFollowers(Graph g, int src, int followers[]) {
	int nV = g -> nV;
	for(int i = 0; i < nV; i++)
		followers[i] = -1;
	followers[src] = 1;
	setfollowers(g, src, followers);
}

void setfollowers(Graph g, int src, int followers[]) {
	for(int v = 0; v < g -> nV; v++) {
		if(dfsPathCheck(g, g -> nV, v, src) && v != src) {
			if(checkInOut(g, v) == 1) followers[v] = 1;
		}
	}
}

int checkInOut(Graph g, int v) {
	int out = 0; int in = 0;
	for(int w = 0; w < g -> nV; w++) {
		if(w != v) {
			if(GraphIsAdjacent(g, v, w)) out++;
			if(GraphIsAdjacent(g, w, v)) in++;
		}
	}
	if(in > out) return 1;
	return 0;
}

bool dfsPathCheck(Graph g, int nV, Vertex v, Vertex dest) {
	int *visited = malloc(nV * sizeof(int));
	for(int i = 0; i < nV; i++)
		visited[i] = -1;
	visited[v] = v;
	bool result = do_dfsPathCheck(g, nV, v, dest, visited);
	free(visited);
	return result;
}
bool do_dfsPathCheck(Graph g, int nV, Vertex v, Vertex dest, int *visited) {
   for(int w = 0; w < nV; w++) {
         if(GraphIsAdjacent(g, v, w) && visited[w] == -1) {
            visited[w] = v;
            if(do_dfsPathCheck(g, nV, w, dest, visited))
               return true;
            if(w == dest)
               return true;
         }
   }
   return false;
}
