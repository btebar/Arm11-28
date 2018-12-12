#ifndef ARM11_28_INSTRUCTION_DEFS_H
#define ARM11_28_INSTRUCTION_DEFS_H

#include <stdint.h>
#include "../usefulTools.h"
#include "defs.h"

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

#endif //ARM11_28_INSTRUCTION_DEFS_H
