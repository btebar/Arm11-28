#ifndef ARM11_28_CELL_H
#define ARM11_28_CELL_H
#include <stdbool.h>

enum status{OFF = 0, ON = 1};

typedef struct{
    enum status s;
} cell;

void flipCell(cell *);

const char *symbol(cell);
#endif //ARM11_28_CELL_H
