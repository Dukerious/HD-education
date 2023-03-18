
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Stack.h"
bool do_hasCycle(Graph g, int nV, bool *visited);
bool checkOnecompo(Graph g, int nV, bool *visited, int v, int prev);
bool hasCycle(Graph g) {
	int nV = GraphNumVertices(g);
	bool *visited = calloc(nV, sizeof(bool));
	bool result = do_hasCycle(g, nV, visited);
	free(visited);
	return result;
}

bool do_hasCycle(Graph g, int nV, bool *visited) {
	for(int v = 0; v < nV; v++) {
		for(int i = 0; i < nV; i++)
			visited[i] = false;
		visited[v] = true;
		if(checkOnecompo(g, nV, visited, v, v)) return true;
	}
	return false;
}

bool checkOnecompo(Graph g, int nV, bool *visited, int v, int prev) {
	for(int w = 0; w < nV; w++) {
		if(GraphIsAdjacent(g, v, w)) {
			if(!visited[w]) {
				visited[w] = true;
				if(checkOnecompo(g, nV, visited, w, v)) return true;
			} else {
				if(w != prev) return true;
			}
		}
	}
	return false;
}

