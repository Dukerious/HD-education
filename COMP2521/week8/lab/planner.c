// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Place.h"
#include "PQ.h"

////////////////////////////////////////////////////////////////////////
// Your task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
double cal_cost(Place a, Place b);
int store_edge(Graph mst, Place cities[], int numCities,
                Place powerPlant, PowerLine powerLines[]);
void g_free(Graph a, Graph b);

int planGrid1(Place cities[], int numCities, Place powerPlant,
              PowerLine powerLines[]) {
    // create a new graph, which insert all possible edges(vC2).
    // the index in the cities[], which is also the index of 
    // vertex in graph.
    Graph g = GraphNew(numCities + 1);
    for(int v = 0; v < numCities; v++) {
        for(int w = v + 1; w < numCities; w++) {
            // found the cost as the weight of edge.
            double weight = cal_cost(cities[v], cities[w]);
            Edge e = {v, w, weight};
            GraphInsertEdge(g, e);
        }
    }
    // since powerPlant doesn't in the array, we have to 
    // processing separately.
    // the index -- numCities, is the index of plant in graph.
    for(int i = 0; i < numCities; i++) {
        double weight = cal_cost(cities[i], powerPlant);
        Edge e = {i, numCities, weight};
        GraphInsertEdge(g, e);
    }
    // found the mini spanning tree.
    Graph mst = GraphMST(g);
    int count = store_edge(mst, cities, numCities, powerPlant, powerLines);
    g_free(g, mst);
    return count;
}
// store the edges to powerLines[], and return the nums.
int store_edge(Graph mst, Place cities[], int numCities,
                Place powerPlant, PowerLine powerLines[]) {
    int index = 0;
    // found each edges with each vertex.
    for(int v = 0; v < numCities; v++) {
        for(int w = v + 1; w < numCities; w++) {
            if(GraphIsAdjacent(mst, v, w)) {
                powerLines[index].p1 = cities[v];
                powerLines[index].p2 = cities[w];
                index++;
            }
        }
    }
    // since powerPlant doesn't in the array, we have to 
    // processing separately.
    // the index -- numCities, is the index of plant in graph.
    for(int w = 0; w < numCities; w++) {
        if(GraphIsAdjacent(mst, numCities, w)) {
            powerLines[index].p1 = powerPlant;
            powerLines[index].p2 = cities[w];
            index++;
        }
    }
    return index;
}
// found the cost between two vertex.
double cal_cost(Place a, Place b) {
    int x = a.x - b.x;
    int y = a.y - b.y;
    return sqrt(pow(x,2) + pow(y, 2));
}
// free all used graph.
void g_free(Graph a, Graph b) {
    GraphFree(a);
    GraphFree(b);
}
////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(Place cities[], int numCities,
              Place powerPlants[], int numPowerPlants,
              PowerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
