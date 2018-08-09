//
// Created by gp2617 on 30/05/18.
//

#include "halt.h"

void halt(MACHINE *machine) { //checked
    machine->c.decodedInstruction->binary = 0;
    machine->c.decodedInstruction->type = Halt;
    machine->c.decodedInstruction->cond = 14;
}
