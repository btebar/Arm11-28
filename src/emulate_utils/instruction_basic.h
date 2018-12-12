
#ifndef C_CODE_INSTRUCTION_BASIC_H
#define C_CODE_INSTRUCTION_BASIC_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../usefulTools.h"

typedef struct { //checked
    instruction_type type;
    int cond;
    word_t opcode;

    word_t offset;

    //registers
    int Rn;
    int Rd;
    int Rs;
    int Rm;

    //flags for operations
    bool A;
    bool I;
    bool P;
    bool S;
    bool L;
    bool U;

    //immediate value
    word_t immediateValue;

    //operand2
    word_t operand2;

    word_t carry;
    SHIFT_CODE shift;
    word_t binary;
} instructions;

#endif //C_CODE_EXECUTE_H
