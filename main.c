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

    const char * testpath = "sample-input1.txt";
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
            }
        }
        // Store address of page p from seg s in page table
        inputsize = getline(&input, &size, initfile);
        if (inputsize == -1) {
            printf("ERROR: Empty line\n");
            return 0;
        } else {
            int p, s, addr, n;
            for (char const *line = input; sscanf(line, " %d %d %d %n", &p, &s, &addr, &n) == 3; line += n) {
                pmem[pmem[s] + p] = addr;
                setBit1(pmem[s] + p, bitmap, masks);
            }
        }
    }
    // Initialization process is done and file no longer needed
    fclose(initfile);

    const char * testpath2 = "sample-input2.txt";
    inputfile = fopen(testpath2, "r");

    if(inputfile == NULL){
        printf("ERROR: Invalid input file path\n");
        return 0;
    } else {
        inputsize = getline(&input, &size, inputfile);
        if (inputsize == -1) {
            printf("ERROR: Empty line\n");
        } else {
            int op, va, n;
            for (char const *line = input; sscanf(line, "%d %d %n", &op, &va, &n) == 2; line += n) {
                printf("Operation: %d, Address: %d\n", op, va);

            }
        }
    }

    fclose(inputfile);

    //printBitmap(bitmap);
    //printMemory(2000, 3000, pmem);


    free(masks);
    free(bitmap);
}
