#include <stdlib.h>
#include "methods.h"

int main(int argc, char * argv[]) {
    // Initialization
    int * bitmap = createBitMap();
    unsigned int * bitmasks = createBitMasks();


    free(bitmasks);
    free(bitmap);
}