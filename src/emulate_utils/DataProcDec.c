#include "DataProcDec.h"

void decodeDATPROC(uint32_t instruction, MACHINE *machine){ //checked
    machine->c.decodedInstruction->binary = instruction;
    machine->c.decodedInstruction->type = DProc;
    machine->c.decodedInstruction->Rn = getBitRange(instruction, 16, 4)& 0xF;
    machine->c.decodedInstruction->Rd = getBitRange(instruction, 12, 15) & 0xF;
    machine->c.decodedInstruction->I = (bool)((instruction >> 25) & 1) ;
    machine->c.decodedInstruction->S = (bool)((instruction >> 20) & 1);
    machine->c.decodedInstruction->operand2 = getBitRange(instruction, 0, 12) &0xFFF;
    machine->c.decodedInstruction->Rm = getBitRange(instruction, 0, 4) &0xF;
    machine->c.decodedInstruction->opcode = getBitRange(instruction, 21, 4) & 0xF;
    machine->c.decodedInstruction->cond = getBitRange(instruction, 28, 4) & 0xF;

}
