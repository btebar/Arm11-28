#ifndef ARM11_28_GRID_H
#define ARM11_28_GRID_H
#include "cell.h"

cell **setupGrid(int, int);

void evolve(cell **, int, int);

void resetGrid(cell **, int, int);

void copyGrid(cell **, cell **, int, int);

#endif //ARM11_28_GRID_H
