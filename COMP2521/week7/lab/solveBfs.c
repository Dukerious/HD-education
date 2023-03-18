// BFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Queue.h"
void find_neighbor(Maze m, Cell c, Queue q, bool **cells, Cell **parents);
void mark_path(Maze m, Cell exit, Cell start, Cell **parents);
void Free_bfs(Queue q, bool **cells, Cell **parents);
bool solve(Maze m) {
    // using cells to mark visited.
    bool **cells = createBoolMatrix(MazeHeight(m), MazeWidth(m));
    // using parents to store parent, for mark path.
    Cell **parents = createCellMatrix(MazeHeight(m), MazeWidth(m));
    Cell start = MazeGetStart(m);
    Queue q = QueueNew();
    QueueEnqueue(q, start);
    // initialize the start.
    cells[start.row][start.col] = true;
    parents[start.row][start.col] = start;
    // do bfs search.
    // until, we met all cells and still not met exit.
    while (!QueueIsEmpty(q)) {
        Cell c = QueueDequeue(q);
        // find neighbor and enqueue.
        find_neighbor(m, c, q, cells, parents);
        // if we find the exit return true.
        if(MazeVisit(m, c)) {
            // mark path.
            mark_path(m, c, start, parents);
            Free_bfs(q, cells, parents);
            return true;
        }
    }
    // not found return.
    Free_bfs(q, cells, parents);
    return false;
}

// check visited and valid cells as well as enqueue, store the parents.
// we will check the neighbor, which are up, down, right and left.
void find_neighbor(Maze m, Cell c, Queue q, bool **cells, Cell **parents) {
    int hight = MazeHeight(m);
    int width = MazeWidth(m);
    int row = c.row;
    int col = c.col;
    // make sure the size of cell is valid.
    if(row - 1 >= 0) {
        // the neighbor of c.
        Cell up = {row - 1, col};
        // if its a valid cell and hasn't visited.
        if(!MazeIsWall(m, up) && !cells[row - 1][col]) {
            cells[row - 1][col] = true;
            parents[row - 1][col] = c;
            QueueEnqueue(q, up);
        }
    }
    if(row + 1 < hight) {
        Cell down = {row + 1, col};
        if(!MazeIsWall(m, down) && !cells[row + 1][col]) {
            cells[row + 1][col] = true;
            parents[row + 1][col] = c;
            QueueEnqueue(q, down);
        }
    }
    if(col - 1 >= 0) {
        Cell left = {row, col - 1};
        if(!MazeIsWall(m, left) && !cells[row][col - 1]) {
            cells[row][col - 1] = true;
            parents[row][col - 1] = c;
            QueueEnqueue(q, left);
        }
    }
    if(col + 1 < width) {
        Cell right = {row, col + 1};
        if(!MazeIsWall(m, right) && !cells[row][col + 1]) {
            cells[row][col + 1] = true;
            parents[row][col + 1] = c;
            QueueEnqueue(q, right);
        }
    }
}

void mark_path(Maze m, Cell exit, Cell start, Cell **parents) {
    // start at exit.
    Cell c = exit;
    // until we meet the start cell.
    while((c.row != start.row) || (c.col != start.col)) {
        MazeMarkPath(m, c);
        c = parents[c.row][c.col];
    }
    // do not forget mark the start.
    MazeMarkPath(m, start);
}
// free the cells and q which we created.
void Free_bfs(Queue q, bool **cells, Cell **parents) {
    freeBoolMatrix(cells);
    freeCellMatrix(parents);
    QueueFree(q);
}

