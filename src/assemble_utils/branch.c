#include <stdint.h>
#include <memory.h>
#include <stdlib.h>
#include "assemblerImplementation.h"
#include "instruction_defs.h"

uint32_t* branch(instruction inst) {
    uint8_t cond_code;
    uint32_t *returnValue = calloc(1, sizeof(uint32_t)) ;

    if (!strcmp(inst.opcode,"beq")) {
        cond_code = 0;
    } else if (!strcmp(inst.opcode, "bne")) {
        cond_code = 1;
    } else if (!strcmp(inst.opcode, "bge"))  {
        cond_code = 10 ;
    } else if (!strcmp(inst.opcode, "blt")) {
        cond_code = 11;
    } else if (!strcmp(inst.opcode, "bgt")) {
        cond_code = 12;
    } else if (!strcmp(inst.opcode,"ble")) {
        cond_code = 13;
    } else {
        cond_code = 14;
    }

    cond_code <<= 4;
    cond_code |= 10;

    // difference of 8 to account for pipeline?
    int offset;

    if (containsLabel(inst.expression)) {
        offset = getAddress(inst.expression)-8;
    } else {
        offset = convertToWriteableFormat(inst.expression)-8;
    }

    offset = offset - (int)(inst.memoryAddr);
    offset = offset >> 2;

    *returnValue = ((cond_code << 24) | offset & 0x00FFFFFF);
    return  returnValue;
}
