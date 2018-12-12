#include "branch.h"



void branch(word_t instruction, MACHINE *machine) {
	machine->c.decodedInstruction->type = Branch;
    word_t offsetValue = (instruction & 0xFFFFFF)<<2;//24 bit offset
		if((instruction>>23)&0x1){ //if the instruction is negative
			offsetValue |= 0xFC000000; //sign extend the offset
		}
		machine->c.decodedInstruction->offset = (offsetValue);
    	machine->c.decodedInstruction->binary = instruction; //saves the binary
		machine->c.decodedInstruction->cond = getBitRange(instruction, condBits, bits_4) & 0xF;

}
