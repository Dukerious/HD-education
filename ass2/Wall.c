// Implementation of the Wall ADT

#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Wall.h"

typedef struct rock Rock;
struct wall {
    int height;
    int width;
    int numrocks;
    Rock *rocks;
};

static int compareRocks(const void *ptr1, const void *ptr2);
static bool WithInDist(int dist, Rock rock, int row, int col);
/**
 * Creates a new blank wall with the given dimensions
 */
Wall WallNew(int height, int width) {
    Wall retval = malloc(sizeof(*retval));
    retval -> height = height;
    retval -> width = width;
    retval -> numrocks = 0;

    retval -> rocks = malloc(sizeof(Rock) * height * width);
    if (retval -> rocks == NULL) {
		fprintf(stderr, "WallNew: Insufficient memory!\n");
		exit(EXIT_FAILURE);
	}
    return retval;
}

/**
 * Frees all memory allocated to the wall 
 */
void WallFree(Wall w) {
    free(w -> rocks);
    free(w);
}

/**
 * Returns the height of the wall
 */
int WallHeight(Wall w) {
    return w -> height;
}

/**
 * Returns the width of the wall
 */
int WallWidth(Wall w) {
    return w -> width;
}

/**
 * Adds a rock to the wall
 * If there is already a rock at the given coordinates, replaces it
 */
void WallAddRock(Wall w, struct rock rock) {

    for (int i = 0; i < w -> numrocks; i++) {
        Rock r = w -> rocks[i];
        if(r.col == rock.col && r.row == rock.row) {
            w -> rocks[i].colour = rock.colour;
            return;
        }
    }
    w -> rocks[w -> numrocks] = rock;
    w -> numrocks ++;
}

/**
 * Returns the number of rocks on the wall
 */
int WallNumRocks(Wall w) {
    return w -> numrocks;
}

/**
 * Stores all rocks on the wall in the given `rocks` array and returns
 * the number of rocks stored. Assumes that the array is at least as
 * large as the number of rocks on the wall.
 */


int WallGetAllRocks(Wall w, struct rock rocks[]) {
    for (int i = 0; i < w -> numrocks; i++) {
        rocks[i].row = w -> rocks[i].row;
        rocks[i].col = w -> rocks[i].col;
        rocks[i].colour = w -> rocks[i].colour;
    }
    return w -> numrocks;
}

/**
 * Stores all rocks that are within a distance of `dist` from the given
 * coordinates in the given `rocks` array and returns the number of rocks
 * stored. Assumes that the array is at least as large as the number of
 * rocks on the wall.
 */

static bool WithInDist(int dist, Rock rock, int row, int col) {
    int distance = sqrt(pow((rock.row - row), 2) + pow((rock.col - col), 2));
    return (distance <= dist);
}

static bool WithInDistAndColor(int dist, Rock rock, Colour color, int row, int col) {
    int distance = sqrt(pow((rock.row - row), 2) + pow((rock.col - col), 2));
    return (distance <= dist && rock.colour == color);
}


int WallGetRocksInRange(Wall w, int row, int col, int dist,
                        struct rock rocks[])
{
    int num = 0;    
    for (int i = 0; i < w -> numrocks; i++) {
        if(WithInDist(dist, w -> rocks[i], row, col)) {
            rocks[i].row = w -> rocks[i].row;
            rocks[i].col = w -> rocks[i].col;
            rocks[i].colour = w -> rocks[i].colour;
            num ++;
        }
    }

    return num;
}

/**
 * Stores all rocks with the colour `colour` that are within a distance
 * of `dist` from the given coordinates in the given `rocks` array and
 * returns the number of rocks stored. Assumes that the array is at
 * least as large as the number of rocks on the wall.
 */
int WallGetColouredRocksInRange(Wall w, int row, int col, int dist,
                                Colour colour, struct rock rocks[])
{
    int num = 0;    
    for (int i = 0; i < w -> numrocks; i++) {
        if(WithInDistAndColor(dist, w -> rocks[i], colour, row, col)) {
            rocks[num].row = w -> rocks[i].row;
            rocks[num].col = w -> rocks[i].col;
            rocks[num].colour = w -> rocks[i].colour;
            num ++;
        }
    }

    return num;
}

////////////////////////////////////////////////////////////////////////

/**
 * Prints the wall out in a nice format
 * NOTE: DO NOT MODIFY THIS FUNCTION! This function will work once
 *       WallGetAllRocks and all the functions above it work.
 */
void WallPrint(Wall w) {
    int height = WallHeight(w);
    int width = WallWidth(w);
    int numRocks = WallNumRocks(w);
    struct rock *rocks = malloc(numRocks * sizeof(struct rock));
    WallGetAllRocks(w, rocks);
    qsort(rocks, numRocks, sizeof(struct rock), compareRocks);

    int i = 0;
    for (int y = height; y >= 0; y--) {
        for (int x = 0; x <= width; x++) {
            if ((y == 0 || y == height) && (x == 0 || x % 5 == 0)) {
                printf("+ ");
            } else if ((x == 0 || x == width) && (y == 0 || y % 5 == 0)) {
                printf("+ ");
            } else if (y == 0 || y == height) {
                printf("- ");
            } else if (x == 0 || x == width) {
                printf("| ");
            } else if (i < numRocks && y == rocks[i].row && x == rocks[i].col) {
                char *color;
                switch (rocks[i].colour) {
                    case GREEN: color = "\x1B[32m"; break;
                    case TEAL:  color = "\x1B[96m"; break;
                    case PINK:  color = "\x1B[95m"; break;
                    case RED:   color = "\x1B[91m"; break;
                    default:    color = "\x1B[0m";  break;
                }
                printf("%so %s", color, RESET);
                i++;
            } else {
                printf("\u00B7 ");
            }
        }
        printf("\n");
    }

    free(rocks);
}

static int compareRocks(const void *ptr1, const void *ptr2) {
    struct rock *r1 = (struct rock *)ptr1;
    struct rock *r2 = (struct rock *)ptr2;
    if (r1->row != r2->row) {
        return r2->row - r1->row;
    } else {
        return r1->col - r2->col;
    }
}

