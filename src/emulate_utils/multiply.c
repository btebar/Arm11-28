#include "multiply.h"


void multiply(word_t instruction, MACHINE *machine){
    machine->c.decodedInstruction->binary = instruction;
	machine->c.decodedInstruction->type = Mult;
    machine->c.decodedInstruction->A = (bool)((instruction >> Abit) & 1);
    machine->c.decodedInstruction->S = (bool)((instruction >> Sbit) & 1);
    machine->c.decodedInstruction->Rn =
                            getBitRange(instruction, RnbitMul, sizeReg)& 0xF;
    machine->c.decodedInstruction->Rd =
                            getBitRange(instruction, RdbitMul, sizeReg) & 0xF;
    machine->c.decodedInstruction->Rs =
                            getBitRange(instruction, RsbitMul, sizeReg) & 0xF;
    machine->c.decodedInstruction->Rm =
                            getBitRange(instruction, RmbitMul, sizeReg) & 0xF;
    machine->c.decodedInstruction->cond =
                            getBitRange(instruction, condBits, bits_4) & 0xF;
}
