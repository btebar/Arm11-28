
#ifndef C_CODE_EXECUTE_H
#define C_CODE_EXECUTE_H

#include "decode.h"
#include <math.h>
#include <byteswap.h>

void execute(MACHINE *machine);

int checkCondition(MACHINE*);

void execute_MulI(MACHINE*);

void execute_Halt(MACHINE*);

void execute_branch(MACHINE*);

void printBitsofReg(int32_t, bool, bool);

void printBitsofMem(uint32_t);

uint32_t signedtwos_to_unsigned(int32_t);

uint32_t shiftReg(uint32_t, MACHINE*);

uint32_t rotate(uint32_t , int);

int binToDec(int);

uint32_t getFromMemory(uint32_t, MACHINE*);

void setMemory(int, uint32_t, MACHINE*);

void execute_SDT(MACHINE*);

void execute_DPI(MACHINE*);

void execute(MACHINE*);


#endif //C_CODE_EXECUTE_H
