
#ifndef ARM11_28_MEMORYIMPLEMENTATION_H
#define ARM11_28_MEMORYIMPLEMENTATION_H

#define PC 15
#define CPSR 16

//holds the state of the program

#include "emulate_utils/instruction_basic.h"

typedef struct{
    uint32_t memoryAlloc[16384];
} MEMORY;

typedef struct{
    uint32_t registers[17];
    uint32_t fetchedInstruction;
    instructions *decodedInstruction;
    bool instructionIsFetched;
} CPU;


typedef struct {
    MEMORY mem;
    CPU c;
} MACHINE;

#endif //ARM11_28_MEMORYIMPLEMENTATION_H
