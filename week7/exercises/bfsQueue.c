#include <stdio.h>
#include <stdbool.h>
#include "queue.h"
#include "Graph.h"

#define MAX_NODES 1000

int visited[MAX_NODES];  // array to store visiting order
                         // indexed by vertex 0..nV-1
bool findPathBFS(Graph g, int nV, Vertex src, Vertex dest) {
   for(int i = 0; i < MAX_NODES; i++)
      visited[i] = -1;
   visited[src] = src;
   queue q = newQueue();
   QueueEnqueue(q, src);
   while(!QueueIsEmpty(q)) {
      int w = QueueDequeue(q);
      for(int v = 0; v < nV; v++) {
         if(adjacent(g, w, v) && visited[v] == -1) {
            QueueEnqueue(q, v);
            visited[v] = w;
            if(v == dest)
               return true;
         }
      }
   }
   return false;
}
/*bool findPathBFS(Graph g, int nV, Vertex src, Vertex dest) {
   Vertex v;
   for (v = 0; v < nV; v++)
      visited[v] = -1;

   visited[src] = src;
   queue Q = newQueue();
   QueueEnqueue(Q, src);
   while (!QueueIsEmpty(Q)) {
      v = QueueDequeue(Q);
      Vertex w;
      for (w = 0; w < nV; w++)
	 if (adjacent(g, v, w) && visited[w] == -1) {
	    visited[w] = v;
	    if (w == dest)
	       return true;
	    else
	       QueueEnqueue(Q, w);
	 }
   }
   return false;
}*/
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

   int src = 8, dest = 0;

   if(findPathBFS(g, V, src, dest)) {
      int v = dest;
      while(v != src) {
         printf("%d - ", v);
         v = visited[v];
      }
      printf("%d\n", src);
   }
   return 0;
}
