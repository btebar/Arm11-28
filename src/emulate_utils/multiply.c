#include "multiply.h"


void multiply(uint32_t instruction, MACHINE *machine){ //checked
    machine->c.decodedInstruction->binary = instruction;
	machine->c.decodedInstruction->type = Mult;
    machine->c.decodedInstruction->A = (bool)((instruction >> 21) & 1);
    machine->c.decodedInstruction->S = (bool)((instruction >> 20) & 1);
    machine->c.decodedInstruction->Rn = getBitRange(instruction, 12, 4)& 0xF;
    machine->c.decodedInstruction->Rd = getBitRange(instruction, 16, 4) & 0xF;
    machine->c.decodedInstruction->Rs = getBitRange(instruction, 8, 4) & 0xF;
    machine->c.decodedInstruction->Rm = getBitRange(instruction, 0, 4) & 0xF;
    machine->c.decodedInstruction->cond = getBitRange(instruction, 28, 4) & 0xF;
}
