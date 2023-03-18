// COMP2521 21T2 Sample Final Exam Q7
// Check graph property

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Constants

#define MAXVERTICES 20
#define MAXLINE 100

// Directed graph as adjacency matrix
// The matrix is *not* symmetric
typedef struct _graph_rep {
	int   nV; 
	bool  edge[MAXVERTICES][MAXVERTICES];
} DiGraph;

// Helper functions
void readGraph(FILE *, DiGraph *);
bool graphIsBinTree(DiGraph);
void showGraph(FILE *, DiGraph);

int main(int argc, char **argv)
{
	DiGraph g;  // graph struct
	FILE *in;   // input file handle

	// handle command-line arguments
	if (argc < 2) {
		fprintf(stderr,"Usage: %s GraphFile\n", argv[0]);
		exit(1);
	}
	if ((in = fopen(argv[1],"r")) == NULL) {
		fprintf(stderr, "Can't read %s\n", argv[1]);
		exit(1);
	}

	// main program
	readGraph(in, &g);
	showGraph(stdout, g);
	printf("\nGraph is ");
	if (!graphIsBinTree(g)) printf("not ");
	printf("a tree\n");
	
	// clean up
	return 0;
}

bool check_incoming_root(DiGraph g, int nV) {
	int root = 0;
	for(int v = 0; v < nV; v++) {
		int incoming = 0;
		for(int w = 0; w < nV; w++) {
			if(g.edge[w][v]) {
				incoming++;
			}
		}
		if(incoming == 0) root++;
		if(incoming > 1) return false;
	}
	return (root == 1);
}
bool check_outgoing(DiGraph g, int nV) {
	for(int v = 0; v < nV; v++) {
		int outgoing = 0;
		for(int w = 0; w < nV; w++) {
			if(g.edge[v][w]) {
				outgoing++;
			}
		}
		if(outgoing > 2) return false;
	}
	return true;
}
bool do_circuit_check(DiGraph g, int nV, int v, int prev, int *visited) {
   for(int w = 0; w < nV; w++) {
      if(g.edge[v][w] || g.edge[w][v]) {
         if(visited[w] != -1) {
            if(w != prev) return true; 
         } else {
            visited[w] = v;
            if(do_circuit_check(g, nV, w, v, visited)) return true;
         }
      }
   }
   return false;
}
bool circuit_check(DiGraph g, int nV, int v, int *visited) {
   for(int i = 0; i < nV; i++)
      visited[i] = -1;
   visited[v] = v;
   return do_circuit_check(g, nV, v, v, visited);
}

// Returns true if the given graph is a binary tree, and false otherwise
bool graphIsBinTree(DiGraph g)
{
	int nV = g.nV;
	if(!check_incoming_root(g, nV)) return false;
	if(!check_outgoing(g, nV)) return false;
	int visited[nV];
	if(circuit_check(g, nV, 0, visited)) return false;
	return true;
}

// Read Graph from "in" file
// Read into DiGraph struct referenced by "g"
void readGraph(FILE *in, DiGraph *g)
{
	char *c, line[MAXLINE];
	int  n, v, nV;
	bool first;

	for (int v = 0; v < MAXVERTICES; v++) {
		for (int w = 0; w < MAXVERTICES; w++)
			g->edge[v][w] = false;
	}
	nV = 0;
	while (fgets(line, MAXLINE, in) != NULL) {
		c = line; first = true;
		while (*c != '\0') {
			// find next number in line
			while (*c != '\0' && !isdigit(*c)) c++;
			if (*c == '\0') break;
			sscanf(c,"%d", &n);
			if (first)
				{ v = n; first = false; }
			else
				g->edge[v][n] = true;
			// skip number we just scan'd
			while (*c != '\0' && isdigit(*c)) c++;
		}
		nV++;
	}
	g->nV = nV;
}

// Print graph info on output file "out"
void showGraph(FILE *out, DiGraph g)
{
	printf("V    Connected to\n");
	printf("--   ------------\n");
	int v, w;
	for (v = 0; v < g.nV; v++) {
		int nout = 0;
		printf("%-3d ",v);
		for (w = 0; w < g.nV; w++) {
			if (g.edge[v][w]) {
				printf(" %d",w);
				nout++;
			}
		}
		if (nout == 0) printf(" -");
		printf("\n");
	}
}
