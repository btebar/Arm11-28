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

struct constantLL {
    uint32_t *label;
    uint32_t address;
    struct constantLL *next;

};


// purposely did not use typedef because typedef breaks a couple of things in the implementation of addLabel in
// assemblerImplementation.c
// all the opcodes, each row is a different type of instruction

static const char *Opcodes[][10] = {
        {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp"},
        {"mul", "mla", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"},
        {"ldr", "str", "\0", "\0", "\0", "\0", "\0", "\0", "\0", "\0"},
        { "beq", "bne", "bge", "blt", "bgt", "ble", "b", "\0", "\0", "\0"}

};


typedef enum {
    DATA_PROCESSING, MULTIPLY, SINGLE_DATA_TRANSFER, BRANCH, LsL, ANDEQ
}instrType ;

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

instruction *decode(char**, uint16_t);

struct symbol *symbolTableHead;
struct constantLL *constantTableHead;
void initSymbolTable(void);

void addLabel (char*, uint16_t) ;

uint16_t getAddress (char*);

uint16_t getLastAddress();

uint32_t getConstantLastAddress();

void clearSymbolTable();

bool containsLabel(char*);

void setInstNull (instruction);

int numberOfLinesInSource;

void initConstantTable(void);
void addConstant(uint32_t*, uint32_t);
uint32_t getConstantAddress(uint32_t*);

void constantsHelperFunction(struct constantLL*);
void clearConstantTable();
bool containsConstant (uint32_t*);
int32_t calculateOffset(uint32_t, uint32_t);


int finalInstAddr;


#endif //ARM11_28_ASSEMBLERIMPLEMENTATION_H
