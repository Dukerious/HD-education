// Implementation of boulder climbing algorithms

#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "climber.h"
#include "Wall.h"
#include "graph.h"

typedef struct rock Rock;
static int StartRockIndexTask1(int row, int col, Colour color, Rock *rocks, int numrocks);
static int StartRockIndexTask2(int row, int col, Rock *rocks, int numrocks);
static int GrabTop(int *top, Rock *rocks, int rocksnum, int wallhight, int reach);
static int GrabRockPath(int *path, Rock *pathrock, Rock *rocks);
static bool CheckValidEdge(Rock v, Rock w, int reach);
static double FindWeight(Rock v, Rock w);
static Graph BuildGraphTask1(Rock *rocks, int numrocks, int reach);
static Graph BuildGraphTask2(Rock *rocks, int numrocks, int reach, int hight, int energyCosts[NUM_COLOURS]);

struct path findShortestPath(Wall w, int reach, Colour colour) {
    struct path p = {0, NULL};

    int numRocks = WallNumRocks(w);
    int wallhight = WallHeight(w);
    int wallwidth = WallWidth(w);

    int maxdist = 2*sqrt(pow(wallhight, 2) + pow(wallwidth, 2));

    Rock *rocks = malloc(numRocks * sizeof(Rock));
    int numRocksWithColor = WallGetColouredRocksInRange(w, 0, 0, maxdist, colour, rocks);

    int *top = malloc(numRocksWithColor * sizeof(int));
    int topnum = GrabTop(top, rocks, numRocksWithColor, wallhight, reach);

    if(topnum == 0) {
        free(rocks);
        free(top);
        return p;
    }
    Graph g = BuildGraphTask1(rocks, numRocksWithColor, reach);

    double min = -1.0;
    int *minpath = NULL;
    for(int i = 1; i < wallwidth; i++) {
        int startindex = StartRockIndexTask1(reach, i, colour, rocks, numRocksWithColor);
        if(startindex != -1) {
            double dis = 0;
            int *path = GraphDijkstra(g, startindex, 0, 0, &dis, top, topnum, false);
            if(dis < min || min == -1.0) {
                min = dis;
                if(minpath != NULL) free(minpath);
                minpath = path;
            }
            else{free(path);}
        }
    }
    

    Rock *pathrock = malloc(numRocksWithColor * sizeof(Rock));

    int numrocksinpath = GrabRockPath(minpath, pathrock, rocks);
    if(numrocksinpath == -1) {
        free(minpath);
        free(pathrock);
        free(rocks);
        free(top);
        GraphFree(g);
        return p;
    }
    p.numRocks = numrocksinpath;
    p.rocks = pathrock;
    free(minpath);
    free(rocks);
    free(top);
    GraphFree(g);
    return p;
}

static int GrabTop(int *top, Rock *rocks, int rocksnum, int wallhight, int reach) {
    int numtop = 0;
    for (int i = 0; i < rocksnum; i++) {
        if(rocks[i].row + reach >= wallhight) {
            top[numtop] = i;
            numtop ++;
        }
    }
    return numtop;
}

static int GrabRockPath(int *path, Rock *pathrock, Rock *rocks) {
    int i;
    for(i = 0; path[i] != -2; i++) {
        int index = path[i];
        if(index == -1) return -1;
        pathrock[i] = rocks[index];
    }

    return i;
}

static Graph BuildGraphTask1(Rock *rocks, int numrocks, int reach) {
    Graph g = GraphNew(numrocks);
    for (int i = 0; i < numrocks; i++) {
        for (int j = 0; j < numrocks; j++) {
            if(CheckValidEdge(rocks[i], rocks[j], reach)) {
                double weight= FindWeight(rocks[i], rocks[j]);
                GraphAddEdge(g, i, j, weight);
            }
        }
    }

    return g;
}

static double FindWeight(Rock v, Rock w) {
    return (sqrt(pow((v.row - w.row), 2) + pow((v.col - w.col), 2)));
}

static bool CheckValidEdge(Rock v, Rock w, int reach) {
    if(v.row == w.row && v.col == w.col) return false;
    return (FindWeight(v, w) <= (double)reach);
}



static int StartRockIndexTask1(int row, int col, Colour color, Rock *rocks, int numrocks) {
    for (int i = 0; i < numrocks; i++) {
        if(rocks[i].row <= row && rocks[i].col == col && rocks[i].colour == color) return i;
    }
    return -1;
}

static int StartRockIndexTask2(int row, int col, Rock *rocks, int numrocks) {
    for (int i = 0; i < numrocks; i++) {
        if(rocks[i].row <= row && rocks[i].col == col) return i;
    }
    return -1;
}

struct path findMinEnergyPath(Wall w, int reach, int energyCosts[NUM_COLOURS]) {
    struct path p = {0, NULL};

    int numRocks = WallNumRocks(w);
    int wallhight = WallHeight(w);
    int wallwidth = WallWidth(w);

    int maxdist = 2*sqrt(pow(wallhight, 2) + pow(wallwidth, 2));

    Rock *rocks = malloc(numRocks * sizeof(Rock));
    int numrocks = WallGetRocksInRange(w, 0, 0, maxdist, rocks);

    int *top = malloc(numrocks * sizeof(int));
    int topnum = GrabTop(top, rocks, numrocks, wallhight, reach);

    if(topnum == 0) {
        free(rocks);
        free(top);
        return p;
    }
    Graph g = BuildGraphTask2(rocks, numrocks, reach, wallhight, energyCosts);

    double min = -1.0;
    int *minpath = NULL;
    for(int i = 1; i < wallwidth; i++) {
        int startindex = StartRockIndexTask2(reach, i, rocks, numrocks);
        if(startindex != -1) {
            double dis = 0;
            Colour startcol = rocks[startindex].colour;
            int *path = GraphDijkstra(g, startindex, 0, 0, &dis, top, topnum, false);
            dis += energyCosts[startcol];
            if(dis < min || min == -1.0) {
                min = dis;
                if(minpath != NULL) free(minpath);
                minpath = path;
            }
            else{free(path);}
        }
    }
    
    Rock *pathrock = malloc(numrocks * sizeof(Rock));

    int numrocksinpath = GrabRockPath(minpath, pathrock, rocks);
    if(numrocksinpath == -1) {
        free(minpath);
        free(pathrock);
        free(rocks);
        free(top);
        GraphFree(g);
        return p;
    }
    p.numRocks = numrocksinpath;
    p.rocks = pathrock;
    free(minpath);
    free(rocks);
    free(top);
    GraphFree(g);
    return p;
}


static Graph BuildGraphTask2(Rock *rocks, int numrocks, int reach, int hight, int energyCosts[NUM_COLOURS]) {
    Graph g = GraphNew(numrocks);
    for (int i = 0; i < numrocks; i++) {
        for (int j = 0; j < numrocks; j++) {
            if(CheckValidEdge(rocks[i], rocks[j], reach)) {
                Colour color = rocks[j].colour;
                double weight= energyCosts[color];
                GraphAddEdge(g, i, j, weight);
            }
        }
    }

    return g;
}

struct path findMinTurnsPath(Wall w, int reach, int energyCosts[NUM_COLOURS],
                             int maxEnergy) {
    struct path p = {0, NULL};

    int numRocks = WallNumRocks(w);
    int wallhight = WallHeight(w);
    int wallwidth = WallWidth(w);

    int maxdist = 2*sqrt(pow(wallhight, 2) + pow(wallwidth, 2));

    Rock *rocks = malloc(numRocks * sizeof(Rock));
    int numrocks = WallGetRocksInRange(w, 0, 0, maxdist, rocks);

    int *top = malloc(numrocks * sizeof(int));
    int topnum = GrabTop(top, rocks, numrocks, wallhight, reach);

    if(topnum == 0) {
        free(rocks);
        free(top);
        return p;
    }
    Graph g = BuildGraphTask2(rocks, numrocks, reach, wallhight, energyCosts);

    double min = -1.0;
    int *minpath = NULL;
    for(int i = 1; i < wallwidth; i++) {
        int startindex = StartRockIndexTask2(reach, i, rocks, numrocks);
        if(startindex != -1) {
            double dis = 0;
            Colour startcol = rocks[startindex].colour;
            int *path = GraphDijkstra(g, startindex, energyCosts[startcol], maxEnergy, &dis, top, topnum, true);
            if(dis < min || min == -1.0) {
                min = dis;
                if(minpath != NULL) free(minpath);
                minpath = path;
            }
            else{free(path);}
        }
    }
    
    Rock *pathrock = malloc(2 * numrocks * sizeof(Rock));

    int numrocksinpath = GrabRockPath(minpath, pathrock, rocks);
    if(numrocksinpath == -1) {
        free(minpath);
        free(pathrock);
        free(rocks);
        free(top);
        GraphFree(g);
        return p;
    }
    p.numRocks = numrocksinpath;
    p.rocks = pathrock;


    free(minpath);
    free(rocks);
    free(top);
    GraphFree(g);

    return p;
}

