//
// Created by nb1317 on 01/06/18.
//

#ifndef ARM11_28_ASSEMBLERIMPLEMENTATION_H
#define ARM11_28_ASSEMBLERIMPLEMENTATION_H

#include <stdint.h>
#include <stdbool.h>
#include <stdint-gcc.h>
#include <stdio.h>
#include "defs.h"


struct symbol {

    char *label;
    uint16_t address;
    struct symbol *next;


};

// this was just extra effort, instead, let there be an overall data structure like the one in emulate which has a subtype type

// purposely did not use typedef because typedef breaks a couple of things in the implementation of addLabel in assemblerImplementation.c
// DO NOT CHANGE TO TYPEDEF STRUCT {..}SYMBOL;

// all the opcodes, not entirely too sure what to do for the special instructions
static const char *Opcodes[][10] = {
        {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp"},
        {"mul", "mla"},
        {"ldr", "str"},
        { "beq", "bne", "bge", "blt", "bgt", "ble", "b"}

};


typedef enum {
    DATA_PROCESSING, MULTIPLY, SINGLE_DATA_TRANSFER, BRANCH, LsL, ANDEQ
}instrType ;

// there should be an enum for instrtype
 typedef struct {
    char *opcode;
    instrType type;

    char *Rd;
    char *Rn;
    char *operand2;

    char *Rm;
    char *Rs;

    char *address;
    char *expression;

    uint16_t memoryAddr;
}instruction;

int countLines(FILE*);
instruction decode(char**, uint16_t);

struct symbol *symbolTableHead;
void initSymbolTable(void);

void addLabel (char*, uint16_t) ;

uint16_t getAddress (char*);

void clearSymbolTable();

bool containsLabel(char*);
uint32_t parse(char*);

void setInstNull (instruction);

int numberOfLinesInSource;

#endif //ARM11_28_ASSEMBLERIMPLEMENTATION_H
