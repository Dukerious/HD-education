typedef int Vertex;



struct edge{
	Vertex w;
	Edge next;
}



typedef struct edge *Edge;

struct graph {
	int nV;
    int nE;
	Edge **edges;
};
typedef struct graph *Graph;

// O(V)
Graph GraphNew(int nv) {
	assert(nV > 0);

    Graph g = malloc(sizeof(*g));
	if (g == NULL) {
		fprintf(stderr, "GraphNew: Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
    g -> nV = nv;
    g -> nE = 0;
    g -> edges = malloc(sizeof(Edge *) * nv);
    if (g->edges == NULL) {
		fprintf(stderr, "GraphNew: Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}

    for (Vertex v = 0; v < nv; v++) {
		g->edges[v] = NULL;
	}


    return g;
}
//O(V)
void InsertEdge(int v, int w, Graph g) {
   DoInsertEdge(v, w, g);
   DoInsertEdge(w, v, g);
}

//O(V)
void DoInsertEdge(int v, int w, Graph g) {
    Edge cur = g -> edges[v];
    if (cur == NULL) g -> edges[v] = MakeNewEdge(w);

    bool found = false;
    while(cur -> next != NULL) {
        if(cur -> w == w) {
            found = true;
            break;
        }
        cur = cur -> next;
    }

    if (!found) cur -> next = MakeNewEdge(w);
}

//O(1)
Edge MakeNewEdge(int w) {
    Edge new = malloc(sizeof(Edge));
    new -> w = w;
    new -> next = NULL;
    return new;
}

//O(V)
void DeleteEdge(int v, int w, Graph g) {
    DoDeleteEdge(v, w, g);
    DoDeleteEdge(w, v, g);
}
//O(V)
void DoDeleteEdge(int v, int w, Graph g) {
    Edge cur = g -> edges[v];
    Edge pre = NULL;
    if (cur -> w == w) {
        g -> edges[v] = cur -> next;
        free(cur);
        return;
    }

    while(cur != NULL) {
        if (cur -> w == w) {
            cur -> next = pre -> next;
            free(cur);
            break;
        }
        Edge pre = cur;
        cur = cur -> next;
    }
}

//O(V)
bool GraphIsAdjacent(int v, int w, Graph g) {
    Edge cur = g -> edges[v][0];
    bool found = false;
    while(cur != NULL) {
        if (cur -> w == w){
            found = true;
            break;
        }
        cur = cur -> next;
    }
    return found;
}