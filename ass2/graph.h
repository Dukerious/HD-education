typedef struct graph *Graph;
typedef int Vertex;
Graph GraphNew(int nV);
void GraphFree(Graph g);
int GraphNumVertices(Graph g);
void GraphAddEdge(Graph g, Vertex v, Vertex w, double weight);
void GraphRemoveEdge(Graph g, Vertex v, Vertex w);
bool GraphIsAdjacent(Graph g, Vertex v, Vertex w);
int *GraphDijkstra(Graph g, int src, int energysrc, int energyfull, double *dis, int *top, int topnum, bool task3);