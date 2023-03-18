// DFS maze solver

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Cell.h"
#include "helpers.h"
#include "Maze.h"
#include "Stack.h"
bool find_neighbor(Maze m, Cell c, Stack s, bool **cells, Cell **parents);
void mark_path(Maze m, Cell exit, Cell start, Cell **parents);
void Free_dfs(Stack s, bool **cells, Cell **parents);
bool solve(Maze m) {
    // using cells to mark visited.
    bool **cells = createBoolMatrix(MazeHeight(m), MazeWidth(m));
    // using parents to store parent, for mark path.
    Cell **parents = createCellMatrix(MazeHeight(m), MazeWidth(m)); 
    // find the cell start and initialize it.
    Cell start = MazeGetStart(m);
    Stack s = StackNew();
    StackPush(s, start);
    cells[start.row][start.col] = true;
    parents[start.row][start.col] = start;
    // do dfs using strack.
    while(!StackIsEmpty(s)) {
        // find the top and mark it.
        Cell c = StackTop(s);
        // if we find exit, return true.
        if(MazeVisit(m, c)) {
            // mark the parth.
            mark_path(m, c, start, parents);
            Free_dfs(s, cells, parents);
            return true;
        }
        // if there is nothing neighbors for c, which we hasn't visited.
        // we will remove it from top.
        if(!find_neighbor(m, c, s, cells, parents)) {
            StackPop(s);
        }
    }
    // not find exit, return false.
    Free_dfs(s, cells, parents);
    return false;
}

// check whether its valid c and doing stack as well as store parent.
// if nothing neighbors which we hasn't visited, return false.
// we will check the neighbor, which are up, down, right and left.
bool find_neighbor(Maze m, Cell c, Stack s, bool **cells, Cell **parents) {
    int hight = MazeHeight(m);
    int width = MazeWidth(m);
    int row = c.row;
    int col = c.col;
    if(row - 1 >= 0) {
        Cell up = {row - 1, col};
        if(!MazeIsWall(m, up) && !cells[row - 1][col]) {
            cells[row - 1][col] = true;
            parents[row - 1][col] = c;
            StackPush(s, up);
            return true;
        }
    }
    if(row + 1 < hight) {
        Cell down = {row + 1, col};
        if(!MazeIsWall(m, down) && !cells[row + 1][col]) {
            cells[row + 1][col] = true;
            parents[row + 1][col] = c;
            StackPush(s, down);
            return true;
        }
    }
    if(col - 1 >= 0) {
        Cell left = {row, col - 1};
        if(!MazeIsWall(m, left) && !cells[row][col - 1]) {
            cells[row][col - 1] = true;
            parents[row][col - 1] = c;
            StackPush(s, left);
            return true;
        }
    }
    if(col + 1 < width) {
        Cell right = {row, col + 1};
        if(!MazeIsWall(m, right) && !cells[row][col + 1]) {
            cells[row][col + 1] = true;
            parents[row][col + 1] = c;
            StackPush(s, right);
            return true;
        }
    }
    return false;
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
// free the cells and stack s, which we just created.
void Free_dfs(Stack s, bool **cells, Cell **parents) {
    freeBoolMatrix(cells);
    freeCellMatrix(parents);
    StackFree(s);
}

