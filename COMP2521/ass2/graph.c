#include "graph.h"
static bool valid_edge(Graph g, Edge e);
static void do_insert_edge(Graph g, Edge e);
static void add_outdegree(Graph g, int v);
static int find_in(Graph g, int v);
static double find_W_in(Graph g, int j, int i);
static double find_W_out(Graph g, int j, int i);
static double find_PR(Graph g, int v);
// struct for graph, store pages' data in queue,
// and each edge in a matrix.
// This graph based on adjecent matrix.
struct graph {
    Queue q;
    int **edges;
    int nV;
    int nE;
};
Graph GraphNew(int nV) {
    assert(nV > 0);
    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g -> nE = 0;
    g->nV = nV;

    g -> q = newQueue(NULL);
    g->edges = malloc(nV * sizeof(int *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(int));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    freeQueue(g -> q);
    free(g);
}

// ensure each insert edge in the graph scope.
static bool valid_edge(Graph g, Edge e) {
    int nV = g -> nV;
    if(e.v >=0 && e.w >=0) {
        return (e.v < nV && e.w < nV);
    }
    return false;
}

void insert_edge(Graph g, char *p1, char *p2) {
    // find the vertex of this page in graph.
    int v = find_vertex(g -> q, p1);
    int w = find_vertex(g -> q, p2);
    assert(v != -1 && w != -1);
    // avoid the reflexive. 
    if(v == w) {
        return;
    }
    Edge e = {v, w};
    do_insert_edge(g, e);
}

static void do_insert_edge(Graph g, Edge e) {
    if(!valid_edge(g, e)) {
        fprintf(stderr, "invalid edge!\n");
        return;
    }
    // If there is no such edge, set the edge.
    if(g -> edges[e.v][e.w] == 0) {
        g -> edges[e.v][e.w] = 1;
    }
    g -> nE++;
    // update the num of outlink for a page, since 
    // we have insert a edge.
    add_outdegree(g, e.v);
}

static void add_outdegree(Graph g, int v) {
    outdegree_updating(g -> q, v);
}

void store_page(Graph g, Page p, bool (*fp) (Page, Page)) {
    QueueEnqueue(g -> q, p, fp);
}
// find the num of inlink for a page by vertex.
static int find_in(Graph g, int v) {
    int count = 0;
    for (int i = 0; i < g -> nV; i++) {
        if(g -> edges[i][v] == 1) {
            count++;
        }
    }
    return count;
}
// find W(in) for a page.
static double find_W_in(Graph g, int j, int i) {
    // find the num of inlinks for page(i).
    int Ipi = find_in(g, i);
    // if numerator is zero, W(in) = 0.
    if(Ipi == 0) {
        return 0;
    }
    // find the sum of reference pages inlinks. 
    int ref_in = 0;
    for(int w = 0; w < g -> nV; w++) {
        if(g -> edges[j][w] == 1) {
            ref_in += find_in(g, w);
        }
    }
    return (double) Ipi / ref_in;
}
// find W(out) for a page.
static double find_W_out(Graph g, int j, int i) {
    // find the num of outlinks for page(i).
    double Opi = (double)found_outdegree(g -> q, i);
    // if its 0, due to avoid the nan, just set 0.5.
    if(Opi == 0) {
        Opi = 0.5;
    }
    // find the sum of reference pages outlinks. 
    double ref_out = 0;
    for(int w = 0; w < g -> nV; w++) {
        if(g -> edges[j][w] == 1) {
            double out = (double)found_outdegree(g -> q, w);
            if(out == 0.0) {
                out = 0.5;
            }
            ref_out += out;
        }
    }
    return (double) Opi / ref_out;
}

// calculate part of PageRank.
double cal_PR(Graph g, char *Pi) {
    int i = find_vertex(g -> q, Pi);
    double sum = 0;
    for(int w = 0; w < g -> nV; w++) {
        if(g -> edges[w][i] == 1) {
            double in = find_W_in(g, w, i);
            double out = find_W_out(g, w, i);
            double PRj = find_PR(g, w);
            sum += in * out * PRj;
        }
    }
    return sum;
}
// find PageRank for a page.
static double find_PR(Graph g, int v) {
    return PR_finding(g -> q, v);
}

// find PageRank difference, and store the new one.
double find_diff(Graph g, char *p, double new_PR) {
    return diff(g -> q, new_PR, p);
}

void print_graph_queue(Graph g) {
    StorePRInFile(g -> q);
}

// debugging function.
void print_graph(Graph g) {
    for(int v = 0; v < g -> nV; v++) {
        for(int w = 0; w < g -> nV; w++) {
            if(g -> edges[v][w] == 1) {
                print_Byvertex(g -> q, v);
                printf(" ->  ");
                print_Byvertex(g -> q, w);
                putchar('\n');
            }
        }
        int outdegree = found_outdegree(g -> q, v);
        printf("the outgree is %d\n", outdegree);
        printf("\n \n\n \n");
    }
}