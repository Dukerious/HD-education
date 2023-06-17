typedef int Vertex;
typedef struct {
    Vertex v;
    Vertex w;
} Edge;


struct graph {
    int nV;
    int nE;
    Edge *edges;
}

typedef struct graph *Graph;

// O(1)
Graph GraphNew(int nv) {
    assert(nv > 0);

    Graph g = malloc(sizeof(*g));
    if(g == NULL) {
        fprintf(stderr, "Error memory, stack overflow!\n");
    }
    g -> nV = nv;
    g -> nE = 0;
    g -> edges = malloc(sizeof(Edge) * nv);

    if(g->edges == NULL) {
        fprintf(stderr, "Error memory, stack overflow!\n");
    }
    return g;
}
// O(E)
void InsertEdge(Graph g, int v, int w) {
    int nE = g -> nE;
    Edge *edge = g -> edges;

    bool find = GraphIsAdjacent(v, w, g);

    if (!find) {
        edges[nE].v = v;
        edge[nE].w = w;
        g -> nE ++;
    }
}
//O(E)
bool GraphIsAdjacent(int v, int w, Graph g) {
    Edge *edges = g -> edges;
    int nE = g -> nE;
    bool find = false;
    for (int e = 0; e < nE; e++) {
        if(edges[e].v == v && edges[e].w == w) {
            find = true;
            break;
        }
    }
    return find;
}
//O(E)
void DeleteEdge(int v, int w, Graph g) {
    Edge *edges = g -> edges;
    int nE = g -> nE;
    bool find = false;
    int target = -1;
    for (int e = 0; e < nE; e++) {
        if(edges[e].v == v && edges[e].w == w) {
            find = true;
            target = e;
            break;
        }
    }
    if(find) {
        edge[target].v = edges[nE].v;
        edge[target].w = edges[nE].w;
        g -> nE --;
    }
}