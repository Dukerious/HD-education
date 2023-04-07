typedef int Vertex;



typedef struct {
	Vertex v;
	Vertex w;
    int weight;
} Edge;

struct graph {
	int nV;
    int nE;
	Edge *edges;
};
typedef struct graph *Graph;

// O(1)
Graph GraphNew(int nv) {
	assert(nV > 0);

    Graph g = malloc(sizeof(*g));
	if (g == NULL) {
		fprintf(stderr, "GraphNew: Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
    g -> nV = nv;
    g -> nE = 0;
    g -> edges = malloc(sizeof(Edge) * nV * nV);
    if (g->edges == NULL) {
		fprintf(stderr, "GraphNew: Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
    return g;
}
//O(E)
void InsertEdge(int v, int w, Graph g) {
    int nE = g -> nE;
    Edge * edges = g -> edges;

    bool find = GraphIsAdjacent(v, w, g);
    if (!find) {
        edges[nE].v = v;
        edges[nE].w = w;
        g -> nE++;
    }
}

//O(E)
void DeleteEdge(int v, int w, Graph g) {
    int nE = g -> nE;
    bool find = false;
    Edge * edges = g -> edges;
    int target = -1;
    for (int e = 0; e < nE; e++) {
        if (edges[e].v == v && edges[e].v == w) {
            find = true;
            target = e;
            break;
        }
    }
    if (find) {
        edges[target].v = edges[nE].v;
        edges[target].w = edges[nE].w;
        g -> nE--;
    }
}

//O(E)
bool GraphIsAdjacent(int v, int w, Graph g) {
    Edge * edges = g -> edges;
    int nE = g -> nE;
    bool find = false;
    for (int e = 0; e < nE; e++) {
        if (edges[e].v == v && edges[e].w == w) {
            find = true;
            break;
        }
    }
    return find;
}
