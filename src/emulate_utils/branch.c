#include "branch.h"



void branch(uint32_t instruction, MACHINE *machine) { //checked
	machine->c.decodedInstruction->type = Branch;
    uint32_t offsetValue = (instruction & 0xFFFFFF)<<2;//24 bit offset
		if((instruction>>23)&0x1){
			//offsetValue = ~offsetValue;
			offsetValue |= 0xFC000000; //sign extend the offset
		}
		machine->c.decodedInstruction->offset = (offsetValue);
    machine->c.decodedInstruction->binary = instruction;
		machine->c.decodedInstruction->cond = getBitRange(instruction, 28, 4) & 0xF;

}
