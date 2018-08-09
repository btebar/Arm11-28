#ifndef ARM11_28_GRID_H
#define ARM11_28_GRID_H
#include "cell.h"
typedef struct {
    cell **board;
} screen;

cell **setupGrid(int, int);

void evolve(screen, int, int);

void evaluateCell(screen, int, int, int, int);

void freeMatrix(cell **);

#endif //ARM11_28_GRID_H
