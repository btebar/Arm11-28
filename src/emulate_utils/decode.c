#include "decode.h"

//Differentiates between the instructions
void decode(MACHINE *machine){
    if (machine->c.fetchedInstruction == 0){
        halt(machine); //if all-zeros is halt
    } else if((machine->c.fetchedInstruction&Branch_MASK)){
        branch(machine->c.fetchedInstruction, machine);
    } else if (machine->c.fetchedInstruction&SDT_MASK){
        SDTdecode(machine->c.fetchedInstruction, machine);
    } else if (((machine->c.fetchedInstruction&I_MASK_32)==0)&&
          ((machine->c.fetchedInstruction&Multiply_MASK1)!=0)&&
          ((machine->c.fetchedInstruction&Multiply_MASK2)!=0)) {
        multiply(machine->c.fetchedInstruction, machine);
    } else {
        decodeDATPROC(machine->c.fetchedInstruction, machine);
    }
    (machine->c.decodedInstruction->binary) = machine->c.fetchedInstruction;

}
