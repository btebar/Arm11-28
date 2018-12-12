#include <stdlib.h>
#include "grid.h"
#include "cell.h"

cell **setupGrid(int width, int height) {
    cell **matrix = malloc(height * sizeof(cell *));

    if (!matrix) {
        return NULL;
    } //Failed

    matrix[0] = (cell *) malloc(width * height * sizeof(cell));

    if (!matrix[0]) {
        free(matrix);
        return NULL;
    } //Failed

    for (int i = 1; i < height; i++) {
        matrix[i] = matrix[i - 1] + width;
    }

    //Initialise grid to all OFF
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            matrix[i][j].s = OFF;
        }
    }

    return matrix;
}

void resetGrid(cell **board, int width, int height) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            board[i][j].s = OFF;
        }
    }
}

int countNeighbours(cell **board, int row, int col, int width, int height) {
    //Count number of neighbours
    int neighbours = 0;

    //Check surrounding cells
    for (int i = -1; i < 2; i++) {
        for (int j = -1; j < 2; j++) {
            //Eliminate bordering issues to each edge can see the other side
            int nCol = (col + j + width) % width;
            int nRow = (row + i + height) % height;
            //Add 1 if neighbour is ON, 0 if OFF
            neighbours += board[nRow][nCol].s;
        }
    }

    //Remove value of this cell as counted neighbour
    neighbours -= board[row][col].s;

    return neighbours;
}


void evolve(cell **grid, int width, int height) {
    //Create a new copy of the board
    cell **new_grid = setupGrid(width, height);
    copyGrid(grid, new_grid, width, height);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int neighbours = countNeighbours(grid, i , j, width, height);
            cell *grid_ptr = &grid[i][j];
            cell *new_ptr = &new_grid[i][j];

            if (grid_ptr->s == ON) {
                if (neighbours < 2 || neighbours > 3) {
                    flipCell(new_ptr);
                }
            } else {
                if (neighbours == 3) {
                    flipCell(new_ptr);
                }
            }
        }
    }

    //Update original grid to new grid
    copyGrid(new_grid, grid, width, height);
}

void copyGrid(cell **grid, cell **new_grid, int width, int height) {
    //Copy over values from one grid to another without altering addresses
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            new_grid[i][j].s = grid[i][j].s;
        }
    }
}