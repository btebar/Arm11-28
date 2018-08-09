//
// Created by nahida on 6/5/18.
//

#ifndef ARM11_28_INSTRUCTION_DEFS_H
#define ARM11_28_INSTRUCTION_DEFS_H

#include <stdint.h>
#include "../usefulTools.h"
#include "defs.h"

typedef struct {
    MNEMONIC mnemonic;
    cond_code cond;
    OPCODE opcode;
    uint8_t Rn;
    uint8_t Rd;
    uint32_t Operand2;
} DataProcInstr_t;

typedef struct {
    MNEMONIC mnemonic;
    cond_code cond;
    char A;
    char S;
    uint8_t Rn;
    uint8_t Rd;
    uint8_t Rs;
    uint8_t Rm;
} MultiplyInstr_t;

typedef struct {
    MNEMONIC mnemonic;
    cond_code cond;
    char P;
    char U;
    char L;
    char I;
    uint8_t Rn;
    uint8_t Rd;
    uint16_t offset;
} SDTinstr_t;

typedef struct {
    MNEMONIC mnemonic;
    cond_code cond;
    uint32_t offset;
} BranchInstr_t;


#endif //ARM11_28_INSTRUCTION_DEFS_H
