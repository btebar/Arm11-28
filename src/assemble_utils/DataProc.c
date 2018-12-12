
#include <memory.h>
#include <stdlib.h>
#include "assemblerImplementation.h"
#include "defs.h"


uint32_t* dataProcessing (instruction inst) {


    uint8_t opcode;
    bool flag = 0 ;
    uint8_t cond = 14;
    uint32_t offset = convertToWriteableFormat(inst.operand2);
    bool flag2 = 1;

    if (inst.operand2[0] == 'r') {
        flag2 = 0;
    } else if ( offset > 0xFF ) {
        offset = getOp2(offset);
    }



    uint32_t *instruction = calloc(1,sizeof(uint32_t));

    if (!strcmp(inst.opcode, "and")) {
        opcode = 0 ;
    } else if (!strcmp(inst.opcode,"eor")) {
        opcode = 1;
    } else if (!strcmp(inst.opcode,"sub")) {
        opcode = 2;
    }else if (!strcmp(inst.opcode,"rsb")) {
        opcode = 3;
    }else if (!strcmp(inst.opcode,"add")) {
        opcode = 4;
    }else if (!strcmp(inst.opcode,"orr")) {
        opcode = 12;
    }else if (!strcmp(inst.opcode,"mov")) {
        opcode = 13;
    }else if (!strcmp(inst.opcode,"tst")) {
        opcode = 8;
        flag = 1;
    }else if (!strcmp(inst.opcode,"teq")) {
        opcode = 9;
        flag = 1;
    }else if (!strcmp(inst.opcode,"cmp")) {
        opcode = 10;
        flag = 1;
    }



    *instruction = (cond << 28 )| (flag2 << 25) | (opcode << 21) | (flag << 20) | convertToWriteableFormat(inst.Rn) << 16
            | (convertToWriteableFormat(inst.Rd) << 12 | offset);
    return instruction;

}

