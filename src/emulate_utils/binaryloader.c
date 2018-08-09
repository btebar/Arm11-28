
#include "binaryloader.h"

// this is just a test comment

void loadFile (char *fname, uint32_t *memory) { //checked
    FILE *f = fopen(fname,"rb");

    if ( f == NULL ) {
        exit(-1);
    }

    size_t size = fread(memory, 16384, 1, f);

    if (ferror(f)) {
        exit(-1);
    }
    fclose(f);
}


//c is 29
