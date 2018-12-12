#ifndef ARM11_28_TET_H
#define ARM11_28_TET_H

typedef struct {
    char **matrix;
    int width, row, col;
} Grid;

Grid copyG(Grid);

void freeGrid(Grid);

int checkPos(Grid);

void getG(void);

void rotate(Grid);

void copyT(void);

void printTable(void);

void changeCur(int);

int checkTime(void);

void runTETRIS(void);

#endif //ARM11_28_TET_H
