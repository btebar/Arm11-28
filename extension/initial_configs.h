#ifndef ARM11_28_INITIAL_CONFIGS_H
#define ARM11_28_INITIAL_CONFIGS_H
#include "cell.h"

void randomConfig(cell **grid, int width, int height);

void periodic(cell **grid, int width, int height);

void geometry(cell **grid, int width, int height);

void butterfly(cell **grid, int width, int height);

void glider(cell **grid, int width, int height);

void gliderGun(cell **grid, int width, int height);

void nightSky(cell **grid, int width, int height);

#endif //ARM11_28_INITIAL_CONFIGS_H
