#include <stdio.h>
#include <stdlib.h>

#include "macros.h"
#include "methods.h"

int main(int argc, char * argv[]) {
    // Variable Declaration
    FILE * initfile;
    FILE * inputfile;
    FILE * outfile;

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
            setBit1(0, bitmap, masks);
            for (char const *line = input; sscanf(line, "%d %d %n", &s, &pt, &n) == 2; line += n) {
                pmem[s] = pt;
                setBit1(s, bitmap, masks);
                setBit1(pmem[s], bitmap, masks);
                setBit1(pmem[s] + FRAMESIZE, bitmap, masks);
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
                setBit1(pmem[pmem[s] + p], bitmap, masks);
            }
        }
    }
    // Initialization process is done and file no longer needed
    fclose(initfile);

    // REMOVED FOR TESTING PURPOSES
    //printf("Input File Path: ");
    //inputsize = getline(&input, &size, stdin);
    //pathtemp = strtok(input, " \n");
    //inputfile = fopen(pathtemp, "r");

    //printf("Output File Path: ");
    //inputsize = getline(&input, &size, stdin);
    //pathtemp = strtok(input, " \n");
    //outfile = fopen(pathtemp, "r");

    const char * testpath2 = "sample-input2.txt";
    inputfile = fopen(testpath2, "r");
    const char * testpath3 = "54565096.txt";
    outfile = fopen(testpath3, "w+");

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


                int s = getS(va);
                int p = getP(va);
                int w = getW(va);

                if(op == READ){
                    if(pmem[s] == -1 || pmem[pmem[s] + p] == -1){
                        fputs("pf ", outfile);
                    } else if (pmem[s] == 0 || pmem[pmem[s] + p] == 0){
                        fputs("err ", outfile);
                    } else {
                        fprintf(outfile, "%d ", pmem[pmem[s] + p] + w);
                    }
                } else if (op == WRITE){
                    printf("SEGMENT: %d PT: %d OFFSET: %d\n", s, p, w);
                    if(pmem[s] == -1 || pmem[pmem[s] + p] == -1){
                        fputs("pf ", outfile);
                    } else if (pmem[s] == 0) {
                        int new_pg = findEmptyFrame(1, bitmap);
                        int new_pt = findEmptyPT(pmem);
                        if (new_pg > 0 && new_pt > 0) {
                            int pt_addr = new_pt * FRAMESIZE;
                            int pg_addr = new_pg * FRAMESIZE;
                            setBit1(pg_addr, bitmap, masks);
                            setBit1(pt_addr, bitmap, masks);
                            setBit1(pt_addr + FRAMESIZE, bitmap, masks);
                            pmem[s] = pt_addr;
                            pmem[pmem[s] + p] = pg_addr;
                            fprintf(outfile, "%d ", pmem[pmem[s] + p] + w);
                        }
                    }else if (pmem[pmem[s] + p] == 0){
                        int new_pg = findEmptyFrame(1, bitmap);
                        if(new_pg > 0){
                            int pg_addr = new_pg * FRAMESIZE;
                            setBit1(pg_addr, bitmap, masks);
                            pmem[pmem[s] + p] = pg_addr;
                            fprintf(outfile, "%d ", pmem[pmem[s] + p] + w);
                        }
                    } else {
                        fprintf(outfile, "%d ", pmem[pmem[s] + p] + w);
                    }
                }
                //for(int i = 0; i < MAPSIZE; ++i){
                //    printf("%d\t\t ", bitmap[i]);
                //}
                //printf("%d | %d | %d | %d | %d \n", va, s, p , w, pmem[pmem[s] + p] + w);

            }
        }
    }

    fclose(inputfile);
    fclose(outfile);

    //printBitmap(bitmap);
    //printMemory(2000, 3000, pmem);



    free(input);
    free(masks);
    free(bitmap);
    free(pmem);
}
