#include "usefulFuncs.h"

//extracts any number of bits from any starting index
word_t getBitRange(word_t instruction, int start, int length){
    //uint32_t num = instruction.binary;
    word_t res = 0;
    for(int i = start; i < start + length; i++){
        res += (1 << i) &instruction;
    }
    return (res >> start);
}

//exiting the program
void exitProgr(MACHINE *machine){
    free(machine->c.decodedInstruction);
    free(machine);
    exit(EXIT_FAILURE);
}
