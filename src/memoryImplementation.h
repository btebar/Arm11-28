
#ifndef ARM11_28_MEMORYIMPLEMENTATION_H
#define ARM11_28_MEMORYIMPLEMENTATION_H

#define UnusedReg1 13
#define UnusedReg2 14
#define PC 15
#define CPSR 16
#define NUM_ADDRESSES 16384
#define NUM_REGISTERS 17

//holds the state of the program
#include "emulate_utils/instruction_basic.h"
#include "usefulTools.h"

typedef struct{
    word_t memoryAlloc[NUM_ADDRESSES];
} MEMORY;

typedef struct{
    word_t registers[NUM_REGISTERS];
    word_t fetchedInstruction;
    instructions *decodedInstruction;
    bool instructionIsFetched;
} CPU;


typedef struct {
    MEMORY mem;
    CPU c;
} MACHINE;

#endif //ARM11_28_MEMORYIMPLEMENTATION_H
