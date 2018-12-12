#include <stdlib.h>
#include "assemblerImplementation.h"
#include <strings.h>
#include <string.h>

// function to initialise the linked list of symbols
void initSymbolTable(void) {
    symbolTableHead = (struct symbol*)(malloc(sizeof(struct symbol)));
    symbolTableHead->label = NULL;
    symbolTableHead->next = NULL;
    symbolTableHead->address = 0;
}

// function to initialise the linked list of constants to be added to the end of the file
void initConstantTable(void) {
    constantTableHead = (struct constantLL*)(malloc(sizeof(struct constantLL)));
    constantTableHead->label = NULL;
    constantTableHead->next = NULL;
    constantTableHead->address = 0;
}

// add a label to the linked list of labels
void addLabel( char* newLabel, uint16_t givAddress ) {
    struct symbol *temp = symbolTableHead;

    while (temp->next!= NULL) {
        temp = temp->next;
    }

    temp->next = (struct symbol*)(malloc(sizeof(struct symbol)));
    temp->next->label = NULL;
    temp->next->address = 0;
    temp->next->next = NULL;
    temp->label = malloc(strlen(newLabel)+1);
    strcpy(temp->label,newLabel);
    temp->address = givAddress;

}

// add a constant to the linked list of constants to be appended to the end of the fild
void addConstant(uint32_t *newLabel, uint32_t givAddress) {
    struct constantLL *temp = constantTableHead;

    while (temp->next!= NULL) {
        temp = temp->next;
    }

    temp->next = (struct constantLL*)(malloc(sizeof(struct constantLL)));
    temp->next->label = NULL;
    temp->next->address = 0;
    temp->next->next = NULL;
    temp->label = malloc(sizeof(uint32_t));
    temp->label = newLabel;
    temp->address = givAddress;

}

// get the address of a label that may or may not exist in the linked list of symbols
uint16_t getAddress(char *givLabel) {
    struct symbol *temp = symbolTableHead;
    while (temp->next != NULL ) {
        if (strcmp(temp->label,givLabel) == 0) {
            return temp->address;
        }
        temp = temp->next;
    }
    return -1;
}

// get the updated last address, taking into account the new constants that have been added to the end of the file
// this function returns the overall last addresss ( in the file )
uint16_t getLastAddress() {
    struct constantLL *temp = constantTableHead;
    int count = 0;
    while (temp->next != NULL ) {
        count+=1;
        temp = temp->next;
    }
    return (count*4 + finalInstAddr);
}

// returns the address of the last constant which has to be added to the end of the file
uint32_t getConstantLastAddress() {
    if (constantTableHead->label == NULL) {
      return getLastAddress();
    } else {
      struct constantLL *temp = constantTableHead;
      while (temp->next != NULL ) {
      }
      return temp->address;
    }
    return -1;
}

// returns the address of a constant that may or may not exist in the linked list of constants to be added to the
// end of the file
uint32_t getConstantAddress(uint32_t *givenLabel) {
    struct constantLL *temp = constantTableHead;
    while (temp->next != NULL ) {
        if (temp->label == *givenLabel) {
            return temp->address;
        }
    }
    return -1;
}

// helper function to free linked list of labels
void helperFunction(struct symbol* something) {
    if (something->next != NULL ) {
        helperFunction(something->next);
    }
    free(something->label);
    free(something);


}

// helper function to free linked list of constants
void constantsHelperFunction(struct constantLL* something) {
    if (something->next != NULL ) {
        constantsHelperFunction(something->next);
    }
    free(something->label);
    free(something);

}

// function to clear the linked list of labels
void clearSymbolTable() {
    if (symbolTableHead->next != NULL) {
        helperFunction(symbolTableHead->next);
    }
   free(symbolTableHead->label);
    free(symbolTableHead);
}

// function to clear the linked list of constants
void clearConstantTable() {
    if (constantTableHead->next != NULL) {
        constantsHelperFunction(constantTableHead->next);
    }
    free(constantTableHead->label);
    free(constantTableHead);
}

// checks if a label exists in the linked list of labels
bool containsLabel(char *givenLabel) {
    struct symbol *temp = symbolTableHead;
    while (temp->next != NULL ) {
        if (strcmp(temp->label, givenLabel) == 0) {
            return true;
        }
        temp = temp->next;
    }
    return false ;
}

// checks if a constant exists in the linked list of constants
bool containsConstant (uint32_t *givenLabel) {
    struct constantLL *temp = constantTableHead;
    while (temp->next != NULL ) {
        if (temp->label == givenLabel) {
            return true;
        }
    }
    return false ;
}

// returns the offset between the constant to the added to the end of the file and the current address of the
// instruction
int32_t calculateOffset(uint32_t PC, uint32_t value) {
    int32_t returnValue;
    uint32_t *key = malloc(sizeof(uint32_t));
    *key = value;
    uint32_t address = getLastAddress();


    addConstant(key, value);
    returnValue = (address) - (int32_t)PC;
    return returnValue&0xFFF;
}

// looks at a tokenized 2D char array and returns a struct instruction based on the same
instruction *decode(char** given, uint16_t memoryAddr) {
    instruction *instr = malloc(sizeof(instruction));

    char *line[5];
    for (int i = 0; i < 5; ++i) {
        line[i] = malloc(strlen(given[i]) + 1);
    }

    for (int i = 0; i < 5; ++i) {
        strcpy(line[i], given[i]);
    }
    instr->opcode = line[0];
    instr->memoryAddr = memoryAddr;


    for (int i = 0; i < 4; ++i) {
        int len = sizeof(Opcodes[i]) / sizeof(Opcodes[i][0]);

        for (int j = 0; j < len; ++j) {
            if (!strcmp(Opcodes[i][j], line[0])) {
                instr->type = i;
                break;
            }

        }

    }

    if (strcmp(line[0], "andeq") == 0) {
        instr->type = 5;
    } else if (strcmp(line[0], "lsl") == 0) {
        instr->type = LsL;
    }

    switch (instr->type) {
        case DATA_PROCESSING:
            if (!strcmp(line[0], "mov")) {
                instr->Rd = line[1];
                instr->Rn = "#0";
                instr->operand2 = line[2];
            } else if (!strcmp(line[0], "tst") || !strcmp(line[0], "teq") || !strcmp(line[0], "cmp")) {
                instr->Rd = "#0";
                instr->Rn = line[1];
                instr->operand2 = line[2];
            } else {
                instr->Rd = line[1];
                instr->Rn = line[2];
                instr->operand2 = line[3];
            }
            break;

        case MULTIPLY:
            instr->Rd = line[1];
            instr->Rm = line[2];
            instr->Rs = line[3];

            if (!strcmp(line[0], "mla")) {
                instr->Rn = line[4];
            }

            break;

        case SINGLE_DATA_TRANSFER:
            instr->Rd = line[1];
            instr->address = line[2];
            //for cases where there is a complex address;

            break;


        case BRANCH:
            instr->expression = line[1];
            break;

        case LsL:
            instr->Rn = line[1];
            instr->expression = line[2];
            break;

        case ANDEQ:
            instr->Rn = '\0';
            instr->Rd = '\0';
            instr->operand2 = '\0';
            break;
    }
    return instr;
}

// returns the number of lines in the binary files, including blank lines
int countLines (FILE* input) {
    int returnVal;
    char line[MAX_LINE_SIZE];


    while(fgets(line, sizeof(line),input)) {
        returnVal++;
    }

    return returnVal;
}

