//
// Created by Blanca Tebar on 03/06/2018.

#include <string.h>
#include "defs.h"
#include "../usefulTools.h"
#include "assemblerImplementation.h"



MNEMONIC takeMnemonic(char *word) {
    if (!strcmp(word, "add")) {
        return add;
    } else if(!strcmp(word, "sub")) {
        return sub;
    } else if(!strcmp(word, "rsb")) {
        return rsb;
    } else if(!strcmp(word, "and")) {
        return and;
    } else if(!strcmp(word, "eor")) {
        return eor;
    } else if(!strcmp(word, "orr")) {
        return orr;
    } else if(!strcmp(word, "mov")) {
        return mov;
    } else if(!strcmp(word, "tst")) {
        return tst;
    } else if(!strcmp(word, "teq")) {
        return teq;
    } else if(!strcmp(word, "cmp")) {
        return cmp;
    } else if(!strcmp(word, "mul")) {
        return mul;
    } else if (!strcmp(word, "mla")) {
        return mla;
    } //to be continued
}


char **tokenizeHelper(char *line) {

    char new_line[MAX_LINE_SIZE];
    char *tokenized[10];
    // 10 is just a random number, but each instruction definitely has less than 10 arguments.
    // if theres only 2 arguments, the rest of them will be set to null so it wont be a problem
    // only have to check for null when we're using the sentence in decode (presumably)
    int i = 0;

    strcpy(new_line, line); /* can't use strtok on string literal */
    //IMPORTANT---------------------------------------------------
    // needed a flag for the pre/post indexing in sdt (PFlag?)
    //look at sdt.c, PFlag in instruction would have to be set if the address
    //is of the form [Rn, <#expression>] instead of [Rn], <#expression>

    tokenized[i] = strtok(new_line, " ");
    while (tokenized[i] != NULL ) {
        i++;
        tokenized[i] = strtok(NULL, ",\n");
    }

    return tokenized;
}


uint32_t *distinguish(instruction inst) {
    uint32_t *returnVal = NULL;

    switch(inst.type) {
        case DATA_PROCESSING:
            returnVal = dataProcessing(inst);
            break;
        case MULTIPLY:
            returnVal = multiply(inst);
            break;

        // HOW DO I HANDLE LSL FOR GODS SAKE WHY DOES IT SAY DUPLICATE CASE VALUE
        case LsL:
            // call to function, where will you convert to uint32?
            break;

        case SINGLE_DATA_TRANSFER:
            returnVal = SDTassembling(inst);
            break;
        case BRANCH:
            returnVal = branch(inst);
            // call to function like this: branch(inst, symbolTable), branch hasnt implemented that
            break;
        case ANDEQ:
            returnVal = 0;
            break;
    }

    return returnVal; 

}



// the idea here is to be able to make a switch function which takes the mnemonic
// and returns, using the defs in usefulTools, the code of both the condition and the opcodes

int32_t convertToWriteableFormat(char* givenStr) {
    uint16_t returnVal = 0 ;

    switch(givenStr[0]) {
        case 'r' :
            // if its a register
            returnVal = textToInt(givenStr);
            break;
        case '#' :
            // this is where we convert Immediates to text
            break;
        case '=' :
            returnVal = textToInt(givenStr);
            break;
    }
    return returnVal;
} // check

uint16_t textToInt(char *givenStr) {
    // the first character of giveStr will definitely be # = or r
    uint16_t returnVal = 0 ;
    int len = strlen(givenStr);

    for (int i = 1 ; i < len ; ++i ) {
        returnVal = returnVal*10 + (givenStr[i] - '0');
    }
    return returnVal;
}

uint32_t getOp2 (int32_t op2) {
    uint16_t shiftVal = 0;
    uint32_t tempVal = op2;
    while (tempVal % 4 == 0) {
        tempVal/=4;
        shiftVal+=2;
    }
    int temp2 = (int)((unsigned)op2 >> shiftVal);
    int temp3 = op2 << (32-shiftVal);

    shiftVal = (32-shiftVal)/2;
    return (shiftVal << 8)/2;
}

uint32_t *lsl(instruction inst) {
    uint32_t *returnValue = calloc(1,sizeof(uint32_t));
    uint8_t condition = 14;
    uint8_t opcode = 13;

    uint32_t Rn = convertToWriteableFormat(inst.Rn);

    uint32_t shiftVal = convertToWriteableFormat(inst.expression);
    uint8_t S = 0;

    *returnValue = ((cond << 28) | opcode << 21 | S << 20 | Rn << 12 | (shiftVal << 7) | Rn );

    return returnValue;

} //fixed

bool checkIfImmediate(char *given) {
    switch(given[0]) {
        case 'r' : return false;
        case '#' :
        case '=' : return true;
    }
    return false;
}


uint32_t shiftOperand (char *base, char *shiftT, char *shiftA) {
    uint32_t returnVal = 0;
    int baseVal = convertToWriteableFormat(base);
    int shiftC = 0;
    int shiftAmount;


    if (!strcmp(shiftT,"lsl")) {
        shiftC = 0;
    } else if (!strcmp(shiftT,"lsr")) {
        shiftC = 1;
    } else if (!strcmp(shiftT,"asr")) {
        shiftC = 2;
    } else if (!strcmp(shiftT,"ror")) {
        shiftC = 3;
    }

    if (checkIfImmediate(shiftA)) {
        shiftAmount = 7;
    } else {
        shiftAmount = 8;
    }

    returnVal = convertToWriteableFormat(shiftA) << shiftAmount | shiftC << 5 | (shiftAmount==8) << 4 | baseVal);

} // fixed
