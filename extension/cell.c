#include "cell.h"
#include <stdio.h>
void flipCell(cell *c) {
    //Change value of cell
    c->s = 1 - c->s;
}

bool alive(cell *c) {
    //Return true if cell is alive
    return c->s == ON;
}

const char *symbol(cell c){
    if (c.s == ON) {
        return "*";
    }
    return "O";
}