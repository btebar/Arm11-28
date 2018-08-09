#include <stdlib.h>
#include "grid.h"
#include "cell.h"

cell **setupGrid(int width, int height) {
    cell **matrix = malloc(width * sizeof(cell *));

    if (!matrix) {
        return NULL;
    } //Failed

    matrix[0] = (cell *) malloc(width * height * sizeof(cell));

    if (!matrix[0]) {
        free(matrix);
        return NULL;
    }

    for (int i = 1; i < width; i++) {
        matrix[i] = matrix[i - 1] + height;
    }

    //Initialise grid to all OFF
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            matrix[i][j].s = OFF;
        }
    }

    return matrix;
}

void freeMatrix(cell **matrix) {
    free(matrix[0]);
    free(matrix);
}

void evolve(screen grid, int width, int height) {
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            evaluateCell(grid, i, j, width, height);
        }
    }
}

void evaluateCell(screen grid, int x, int y, int width, int height) {
    //Count number of neighbours
    int neighbours = 0;

    //Check surrounding cells
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            //If not this cell
            if (!((x + i) < 0 || (x + i) >= width || (y + j) < 0 || (y + j) >= height)){
                if (!(i == 0 && j == 0)) {
                    if (grid.board[x + i][y + j].s == ON) {
                        neighbours += 1;
                    }
                }
            }
        }
    }

    cell *cell_ptr = &grid.board[x][y];

    if (cell_ptr->s == ON) {
        if (neighbours < 2 || neighbours > 3) {
            flipCell(cell_ptr);
        }
    } else {
        if (neighbours == 3) {
            flipCell(cell_ptr);
        }
    }
}