#include "singleDT.h"

void SDTdecode(uint32_t instruction, MACHINE *machine) { //checked
    machine->c.decodedInstruction->binary = instruction;
	machine->c.decodedInstruction->type = SDT;
    machine->c.decodedInstruction->I = (bool)((instruction >> 25) & 1);
    machine->c.decodedInstruction->P = (bool)((instruction >> 24) & 1);
    machine->c.decodedInstruction->U = (bool)((instruction >> 23) & 1);
    machine->c.decodedInstruction->L = (bool)((instruction >> 20) & 1);
    machine->c.decodedInstruction->Rn = getBitRange(instruction, 16, 4)& 0xF;
    machine->c.decodedInstruction->Rd = getBitRange(instruction, 12, 15)& 0xF;
    machine->c.decodedInstruction->offset = getBitRange(instruction, 0, 12) & 0xFFF;
    machine->c.decodedInstruction->cond = getBitRange(instruction, 28, 4) & 0xF;
    machine->c.decodedInstruction->Rm = getBitRange(instruction, 0, 4) &0xF;
}
