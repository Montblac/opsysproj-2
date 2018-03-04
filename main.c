#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "macros.h"
#include "methods.h"

int main(int argc, char * argv[]) {
    // Variable Declaration
    FILE * initfile;
    FILE * inputfile;

    size_t size;
    ssize_t inputsize;
    char * input = NULL;
    char * pathtemp;

    // Initialize bit map, bit masks, physical memory
    int * bitmap = createBitMap();
    int * masks  = createBitMasks();
    int * pmem   = createPhysicalMem();

    // Initialize page tables and pages

    // ## REMOVED FOR TESTING PURPOSES
    //printf("Init File Path: ");
    //inputsize = getline(&input, &size, stdin);
    //pathtemp = strtok(input, " \n");
    //initfile = fopen(pathtemp, "r");

    const char * testpath = "/Users/Sam/Desktop/test.txt";
    initfile = fopen(testpath, "r");

    if(initfile == NULL){
        printf("ERROR: Invalid initialization file path\n");
        return 0;
    } else {
        // Store starting address of page table f in segment s
        inputsize = getline(&input, &size, initfile);
        if (inputsize == -1) {
            printf("ERROR: Empty line\n");
        } else {
            int s, pt, n;
            for (char const *line = input; sscanf(line, "%d %d %n", &s, &pt, &n) == 2; line += n) {
                pmem[s] = pt;
                setBit1(s, bitmap, masks);
                setBit1(pt, bitmap, masks);
                setBit1(pt + 1, bitmap, masks);
            }
        }
        // Store pages p from segment s in the appropriate address addr
        inputsize = getline(&input, &size, initfile);
        if (inputsize == -1) {
            printf("ERROR: Empty line\n");
            return 0;
        } else {
            int p, s, addr, n;
            for (char const *line = input; sscanf(line, " %d %d %d %n", &p, &s, &addr, &n) == 3; line += n) {
                pmem[pmem[s] + p] = addr;
            }
        }
    }
    // Initialization process is done and file no longer needed
    fclose(initfile);

    for(int i = 0; i < MAPSIZE; ++i){
        printf("%u\n", bitmap[i]);
    }

    free(masks);
    free(bitmap);
}