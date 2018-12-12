
#include "binaryloader.h"

//Loads the binary file
void loadFile (char *fname, word_t *memory) {
    FILE *f = fopen(fname,"rb");

    if (f == NULL) {
        exit(-1);
    }

    size_t size = fread(memory, NUM_ADDRESSES, 1, f);

    if (ferror(f)) {
        exit(-1);
    }
    fclose(f);
}
