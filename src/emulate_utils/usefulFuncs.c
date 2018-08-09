#include "usefulFuncs.h"

//extracts any number of bits from any starting index

//uint32_t getBitRange(uint32_t num, int start, int length){

uint32_t getBitRange(uint32_t instruction, int start, int length){ //checked
    //uint32_t num = instruction.binary;
    uint32_t res = 0;
    for(int i = start; i < start + length; i++){
        res += (1 << i) &instruction;
    }
    return (res >> start);
}

void exitProgr(MACHINE *machine){
    free(machine->c.decodedInstruction);
    free(machine);
    exit(EXIT_FAILURE);
}


