#include <string.h>
#include <stdlib.h>
#include "defs.h"
#include "../usefulTools.h"
#include "assemblerImplementation.h"

// takes a line of text from the binary file and parses into a 2D char array which can then be easily passed to decode
char **tokenizeHelper(char *line) {
    char *tempLine;
    char *tempLine2 = NULL;

    char newline[MAX_LINE_SIZE];
    char **tokenized = malloc(sizeof(char*)*10);


    int i = 0;
    strcpy(newline,line);


    if (line[strlen(line)-1] == '\n') {
        line[strlen(line)-1] = '\0';
    }

    strcpy(newline,line);

    if (strchr(line,'[') != NULL) {
        tempLine = strtok(newline,"[");
        tempLine2 = strtok(NULL,"[");
    } else {
        tempLine = newline;
    }
    // getting rid of the [ so that you can get rid of the ] in SDT


    tempLine = strtok(tempLine, ", ");
    // keep splitting as you go along while also looking at commas

    for (; tempLine != NULL; i++) {
        tokenized[i] = tempLine;
        tempLine = strtok(NULL,", ");
    }

    // in case there are nested instructions

    if ( tempLine2 != NULL && i < 10) {
        tokenized[i] = tempLine2;
        i++;
    }

    for ( ; i < 10 ; i++ ) {
        tokenized[i] = "";
    }

    return tokenized;
}

//looks at the type of instruction, and calls the respective instructin.
uint32_t *distinguish(instruction inst) {
    uint32_t *returnVal;

    switch (inst.type) {
        case DATA_PROCESSING:
            returnVal = dataProcessing(inst);
            break;
        case MULTIPLY:
            returnVal = multiply(inst);
            break;
        case SINGLE_DATA_TRANSFER:
            returnVal = SDTassembling(inst);
            break;

        case BRANCH:
            returnVal = branch(inst);
            break;

        case LsL:
            returnVal = lslFunc(inst);
            break;

        case ANDEQ:
            break;
    }

    return returnVal;

}

// convert from text to int32, so that it can be used in shift and written into the binary file
int32_t convertToWriteableFormat(char *givenStr) {
    int32_t returnVal = 0;
    int32_t temp = 1;
    bool neg = false;
    switch(givenStr[0]) {
        case 'r' : returnVal = atoi(givenStr+1); break;
        case '#' :
        case '=' :
            if (givenStr[1] == '-') {
                temp = 2;
                neg = true;
            }
            if (givenStr[temp] == '0' && givenStr[temp+1] == 'x') {
                returnVal = (int32_t)strtol(givenStr+1,NULL, 0);
            } else {
                returnVal = atoi(givenStr+1);
            }
            break;
    }
    if (neg) {
      returnVal *= (-1);
    }
    return returnVal;
}

// checks if an immediate is negative
bool isNeg(char *givenStr) {
    bool neg = false;
    switch(givenStr[0]) {
        case 'r' :
            break;
        case '#' :
        case '=' :
          if (givenStr[1] == '-'){
            neg = true;
          }
            break;
        default:
              break;
            }
    return neg;
}

// returns operand2 in special cases of SDT
uint32_t getOp2 (int32_t op2) {
    int shiftVal = 32;
    uint32_t tempVal = op2;
    while(!(tempVal & 0x3)){
      tempVal >>= 2;
      shiftVal--;
    }
    return ((shiftVal << 8) | (tempVal & 0xFF)) & 0xFFF;
}

//Effect of instruction LSL
uint32_t *lslFunc(instruction inst) {
    uint32_t *returnValue = calloc(1,sizeof(uint32_t));
    uint8_t condition = 14;
    uint8_t opcode = 13;

    uint32_t Rn = convertToWriteableFormat(inst.Rn);

    uint32_t shiftVal = convertToWriteableFormat(inst.expression);
    uint8_t S = 0;

    *returnValue = ((condition << 28) | opcode << 21 | S << 20 | Rn << 12 | (shiftVal << 7) | Rn );

    return returnValue;
}

// checks if a given string contains a constant value
bool checkIfImmediate(char *text) {
  char *given = malloc(strlen(text));
  strcpy(given,text);
    switch(given[0]) {
        case 'r' :
                return false;
        case '#' :
        case '=' :
                return true;
        default:
                return false;
    }

}

// shifts a given value based on the type and amount
uint32_t shiftOperand (char *base, char *shiftT, char *shiftA) {

    uint32_t returnVal = 0;
    int baseVal = convertToWriteableFormat(base);
    int shiftC = 0;
    int shiftAmount;


    if (strcmp(shiftT, "lsl") == 0) {
        shiftC = 0;
    } else if (strcmp(shiftT, "lsr") == 0) {
        shiftC = 1;
    } else if (strcmp(shiftT, "asr") == 0) {
        shiftC = 2;
    } else if (strcmp(shiftT, "ror") == 0) {
        shiftC = 3;
    }
    // to check if the shift amount is an immediate value

    if (checkIfImmediate(shiftA)) {
        shiftAmount = 7;
    } else {
        shiftAmount = 8;
    }

    returnVal = convertToWriteableFormat(shiftA) << shiftAmount | shiftC << 5 |
     (shiftAmount==8) << 4 | baseVal;

    return returnVal;
}
