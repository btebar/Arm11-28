#include "DataProcDec.h"

void decodeDATPROC(word_t instruction, MACHINE *machine){

    //SavE the different files of the instruction
    machine->c.decodedInstruction->binary = instruction;
    machine->c.decodedInstruction->type = DProc;
    machine->c.decodedInstruction->Rn =
                                getBitRange(instruction, Rnbit, sizeReg)& 0xF;
    machine->c.decodedInstruction->Rd =
                                getBitRange(instruction, Rdbit, sizeReg) & 0xF;
    machine->c.decodedInstruction->I = (bool)((instruction >> Ibit) & 1) ;
    machine->c.decodedInstruction->S = (bool)((instruction >> Sbit) & 1);
    machine->c.decodedInstruction->operand2 =
                                getBitRange(instruction, 0, sizeOffset) &0xFFF;
    machine->c.decodedInstruction->Rm =
                                getBitRange(instruction, 0, sizeReg) &0xF;
    machine->c.decodedInstruction->opcode =
                                getBitRange(instruction, OpcodeBit, sizeopcode)
                                                                        & 0xF;
    machine->c.decodedInstruction->cond =
                                getBitRange(instruction, condBits, bits_4) & 0xF;

}
