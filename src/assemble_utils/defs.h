//
// Created by Blanca Tebar on 03/06/2018.
//

#ifndef ARM11_28_DEFS_H
#define ARM11_28_DEFS_H

#include <stdint.h>
#include <stdint-gcc.h>
#include "DataProc.h"
#include "branch.h"
#include "multiply.h"
#include "sdt.h"
#include "assemblerImplementation.h"

#define MAX_LINE_SIZE 511

typedef enum {
    add,
    sub,
    rsb,
    and,
    eor,
    orr,
    mov,
    tst,
    teq,
    cmp,
    mul,
    mla,
    ldr,
    str,
    beq,
    bne,
    bge,
    blt,
    bgt,
    ble,
    b,
    lsl,
    andeq
} MNEMONIC;

char **tokenizeHelper(char*);

uint32_t *distinguish(instruction);

uint32_t getOp2(int32_t);

int32_t convertToWriteableFormat(char*);

uint32_t *lslFunc(instruction);

bool checkIfImmediate(char*);

uint32_t shiftOperand(char*, char*, char*);

uint16_t textToInt(char*);

char *removeSpaces(char*);
#endif //ARM11_28_DEFS_H
