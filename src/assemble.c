#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include "memoryImplementation.h"
#include "emulate_utils/instruction_basic.h"
#include "assemble_utils/assemblerImplementation.h"
#include "assemble_utils/defs.h"

int main(int argc, char **argv) {

    char *sourceFile = argv[1];

    char *destFile = argv[2];

    uint32_t address = 0;

    char line[MAX_LINE_SIZE];

    //First pass
    FILE *input;
    input = fopen(argv[1],"r");
    if (input == NULL) {
        printf("Could not open the file");
        exit(-1);
    }

    const int numLines = countLines(input);
    numberOfLinesInSource = numLines;
    instruction *instArr = calloc(numLines,sizeof(instruction));

    rewind(input);
    initSymbolTable();
    initConstantTable();

    uint32_t *valueToBeWritten = NULL;
    while (fgets(line, sizeof(line), input)) {
        if (line[0] != '\n' && line[0] != '\0' && line[0] != ' ') {
            // this gets rid of empty lines like the ones in beq
            char **tokenizedLine = tokenizeHelper(line);

            /* tokenizeHelper takes a line and return a 2D char array where the first sentence will be the instruction or label
             it will be a label iff the last char of the first line is :
             otherwise send it to decode;*/

            if (tokenizedLine[0][strlen(tokenizedLine[0]) - 1] == ':') {
                // if its a label
                char *label = malloc(sizeof(tokenizedLine[0]));
                strcpy(label, tokenizedLine[0]);
                label[strlen(label) - 1] = '\0';

                // replace the : with a NULL character
                if (containsLabel(label) == 0) {
                    // only add to the symbol if it doesnt already exist in there
                    addLabel(label, address * 4);
                }
            } else {
                // its an instruction, so decode it
                instruction *temp = decode(tokenizedLine, address * 4);
                memcpy(instArr + address, temp, sizeof(instruction));

                address += 1;
            }
        }
    }

    finalInstAddr = 4*address;
    fclose(input);

    FILE *output;
    if ((output = fopen(argv[2],"wb"))==NULL) {
        printf("Could not open output file");
        exit(-1);
    }

    //Second pass
    uint32_t f = 0;
    for ( int i = 0; i < address; ++i) {
        uint32_t *writeValue = distinguish(instArr[i]);

        if ( instArr[i].type == ANDEQ ) {
            //doing this because other methods to set the value of a pointer to zero would result in a NULL pointer
            fwrite(&f,1,sizeof(uint32_t),output);
        } else {
            fwrite(writeValue,1, sizeof(uint32_t),output);
        }
    }

    // take all constants present in constant table, and append at the end of the file
    if (constantTableHead->label!=NULL) {
        struct constantLL *temp = constantTableHead;

        while (temp->label != NULL) {
            fwrite(&temp->address, 1, sizeof(uint32_t), output);
            temp = temp->next;
        }
    }

    fclose(output);
    clearSymbolTable();
    clearConstantTable();

    return EXIT_SUCCESS;
}
