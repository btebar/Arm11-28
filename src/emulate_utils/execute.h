
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

void printBitsofMem(word_t);

word_t signedtwos_to_unsigned(int32_t);

word_t shiftReg(word_t, MACHINE*);

word_t rotate(word_t , int);

int binToDec(int);

word_t getFromMemory(word_t, MACHINE*);

void setMemory(int, word_t, MACHINE*);

void execute_SDT(MACHINE*);

void execute_DPI(MACHINE*);

void execute(MACHINE*);

bool isGPIO(uint32_t,MACHINE*);
#endif //C_CODE_EXECUTE_H
