#include <stdio.h>
#include <stdbool.h>
#include "Graph.h"
#include <stdlib.h>
#define MAX_NODES 1000
int visited[MAX_NODES];  // array to store visiting order
                         // indexed by vertex 0..nV-1


                         
bool dfsPathCheck(Graph g, int nV, Vertex v, Vertex dest) {
   if (v == dest) return true;

   for (int w = 0; w < nV; w++) {
      if (adjacent(g, v, w) && visited[w] == -1) {
         visited[w] = v;
         if(dfsPathCheck(g, nV, w, dest)) return true;
      }
   }
   return false;
}


bool circuit_check(Graph g, int nV, Vertex v) {
   for (int i = 0; i < MAX_NODES; i++) 
      visited[i] = -1;
   visited[v] = v;
   return do_circuit_check(g, nV, v, v);
}

bool do_circuit_check(Graph g, int nV, Vertex v, Vertex prev) {
   for(int w = 0; w < nV; w++) {
      if(adjacent(g, v, w)) {
         if(visited[w] != -1) {
            if(w != prev) return true;
         } else {
            visited[w] = v;
            if(do_circuit_check(g, nV, w, v)) return true;
         }
      }
   }
   return false;
}


bool hascycle(Graph g, int nV) {
   for(int v = 0; v < nV; v++) {
      if(circuit_check(g, nV, v))
         return true;
   }
   return false;
}

int *marking_components(Graph g, int nV) {
   int *componentof = malloc(nV*sizeof(int));
   for (int i = 0; i < nV; i++) {
      componentof[i] = -1;
   }

   int id = 0;

   for (int v = 0; v < nV; v++) {
      if (componentof[v] == -1) {
         componentof[v] = id;
         do_components(g, nV, v, id, componentof);
         id++;
      }
      return componentof;
   }
}

void do_components(Graph g, int nV, int v, int id, int *componentof) {
   for (int w = 0; w < nV; w++) {
      if(adjacent(g, v, w) && componentof[w] == -1) {
         componentof[w] = id;
         do_components(g, nV, w, id, componentof);
      }
   }
}

int main(void) {
   int V = 9;
   Graph g = newGraph(V);

   Edge e;
   e.v = 0; e.w = 3; insertEdge(g, e);
   e.v = 3; e.w = 1; insertEdge(g, e);
   e.v = 3; e.w = 5; insertEdge(g, e);
   e.v = 1; e.w = 7; insertEdge(g, e);
   e.v = 5; e.w = 4; insertEdge(g, e);
   e.v = 5; e.w = 2; insertEdge(g, e);
   e.v = 4; e.w = 6; insertEdge(g, e);
   e.v = 2; e.w = 6; insertEdge(g, e);
   e.v = 7; e.w = 2; insertEdge(g, e);
   e.v = 7; e.w = 8; insertEdge(g, e);
   e.v = 6; e.w = 8; insertEdge(g, e);

   int src = 5, dest = 0;
   if (dfsPathCheck(g, V, src, dest)) {
      Vertex v = dest;
      while (v != src) {
	 printf("%d - ", v);
	 v = visited[v];
      }
      printf("%d\n", src);
   }
   return 0;
}
