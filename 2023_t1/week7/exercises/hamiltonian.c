#include <stdio.h>
#include <stdbool.h>
#include "Graph.h"

#define MAX_NODES 1000
bool visited[MAX_NODES];

bool dohasHamiltonianPath(Graph g, int nV, int src, int dest, int d);
bool hasHamiltonianPath(Graph g, int nV, Vertex src, Vertex dest) {
   for (int v = 0; v < nV; v++)
      visited[v] = false;
   visited[src] = true;

   return dohasHamiltonianPath(g, nV, src, dest, nV - 1);
}

bool dohasHamiltonianPath(Graph g, int nV, int src, int dest, int d) {
   if(src == dest) return (d == 0);
   visited[src] = true;

   for (int v = 0; v < nV; v++) {
      if(adjacent(g, src, v) && !visited[v]) {
         if(dohasHamiltonianPath(g, nV, v, dest, d - 1)) return true;
      }
      
   }
   visited[src] = false;
   return false;
}



int main(void) {
   Edge e;
   int n;

   printf("Enter the number of vertices: ");
   scanf("%d", &n);
   Graph g = newGraph(n);

   Vertex src, dest;
   printf("Enter source node: ");
   scanf("%d", &src);
   printf("Enter destination node: ");
   scanf("%d", &dest);
   
   printf("Enter an edge (from): ");
   while (scanf("%d", &e.v) == 1) {
      printf("Enter an edge (to): ");
      scanf("%d", &e.w);
      insertEdge(g, e);
      printf("Enter an edge (from): ");
   }
   printf("Finished.\n");

   printf("The graph has ");
   if (hasHamiltonianPath(g, n, src, dest))
      printf("a");
   else
      printf("no");
   printf(" Hamiltonian path from %d to %d.\n", src, dest);

   freeGraph(g);
   return 0;
}
