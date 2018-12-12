#include "singleDT.h"

void SDTdecode(word_t instruction, MACHINE *machine) {
    machine->c.decodedInstruction->binary = instruction;
	machine->c.decodedInstruction->type = SDT;
    machine->c.decodedInstruction->I = (bool)((instruction >> Ibit) & 1);
    machine->c.decodedInstruction->P = (bool)((instruction >> Pbit) & 1);
    machine->c.decodedInstruction->U = (bool)((instruction >> Ubit) & 1);
    machine->c.decodedInstruction->L = (bool)((instruction >> Lbit) & 1);
    machine->c.decodedInstruction->Rn =
                                getBitRange(instruction, Rnbit, sizeReg)& 0xF;
    machine->c.decodedInstruction->Rd =
                                getBitRange(instruction, Rdbit, sizeReg)& 0xF;
    machine->c.decodedInstruction->offset =
                                getBitRange(instruction, 0, sizeOffset) & 0xFFF;
    machine->c.decodedInstruction->cond =
                                getBitRange(instruction, condBits, bits_4) & 0xF;
    machine->c.decodedInstruction->Rm =
                                getBitRange(instruction, 0, sizeReg) &0xF;
}
