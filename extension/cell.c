#include "cell.h"
#include <stdio.h>
void flipCell(cell *c) {
    //Change value of cell
    c->s = 1 - c->s;
}

const char *symbol(cell c){
    if (c.s == ON) {
        return "*";
    }
    return " ";
}