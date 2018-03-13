#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

#include "macros.h"
#include "methods.h"

int main(int argc, char * argv[]) {
    // Variable Declaration
    FILE * initfile, * inputfile, * outfile, * outfile2;
    int * bitmap, * masks, * pmem;

    size_t size;
    ssize_t inputsize;
    char * input = NULL;
    char * pathtemp;


    // Initialize bit map, bit masks, physical memory
    init(&bitmap, &masks, &pmem);

    // Initialize page tables and pages

    // ## REMOVED FOR TESTING PURPOSES
    //printf("Init File Path: ");
    //inputsize = getline(&input, &size, stdin);
    //pathtemp = strtok(input, " \n");
    //initfile = fopen(pathtemp, "r");

    //printf("Input File Path: ");
    //inputsize = getline(&input, &size, stdin);
    //pathtemp = strtok(input, " \n");
    //inputfile = fopen(pathtemp, "r");

    //printf("Non-TLB Output File Path: ");
    //inputsize = getline(&input, &size, stdin);
    //pathtemp = strtok(input, " \n");
    //outfile = fopen(pathtemp, "r");

    //printf("TLB Output File Path: ");
    //inputsize = getline(&input, &size, stdin);
    //pathtemp = strtok(input, " \n");
    //outfile2 = fopen(pathtemp, "r");

    // ## FOR TESTING PURPOSES
    const char * testpath = "student_test_init.txt"; //"sample-input1.txt";
    initfile = fopen(testpath, "r");
    const char * testpath2 = "student_test_va2.txt"; //"sample-input2.txt";
    inputfile = fopen(testpath2, "r");
    const char * testpath3 = "54565096.txt";
    outfile = fopen(testpath3, "w+");
    const char * testpath4 = "54565096_tlb.txt";
    outfile2 = fopen(testpath4, "w+");


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

                int w = getW(va);
                int s = getS(va);
                int p = getP(va);

                if(op == READ){
                    if(pmem[s] == -1 || pmem[pmem[s] + p] == -1){
                        fputs("pf ", outfile);
                    } else if (pmem[s] == 0 || pmem[pmem[s] + p] == 0){
                        fputs("err ", outfile);
                    } else {
                        fprintf(outfile, "%d ", pmem[pmem[s] + p] + w);
                    }
                } else if (op == WRITE){
                    if(pmem[s] == -1 || pmem[pmem[s] + p] == -1){
                        fputs("pf ", outfile);
                    } else if (pmem[s] == 0) {

                        int new_pt = findEmptyPT(bitmap);
                        if(new_pt > 0){
                            int pt_addr = new_pt * FRAMESIZE;
                            setBit1(pt_addr, bitmap, masks);
                            setBit1(pt_addr + FRAMESIZE, bitmap, masks);
                            pmem[s] = pt_addr;
                            int new_pg = findEmptyFrame(1, bitmap);
                            if(new_pg > 0){
                                int pg_addr = new_pg * FRAMESIZE;
                                setBit1(pg_addr, bitmap, masks);
                                pmem[pmem[s] + p] = pg_addr;
                                fprintf(outfile, "%d ", pmem[pmem[s] + p] + w);
                            }
                        }
                    } else if (pmem[pmem[s] + p] == 0){
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
            }
        }
    }
    // Outfile for Non-TLB is completed
    fclose(outfile);



    // -------------------------- START OF TLB VERSION ------------------------------



    // Clearing bitmap, masks, and physical memory
    free(masks);
    free(bitmap);
    free(pmem);

    // Reinitialize bitmap, masks, and physical memory
    init(&bitmap, &masks, &pmem);

    // Initialize TLB
    int tlb[4][3] = {0};
    for(int i = 0; i < 4; ++i){
        tlb[i][0] = i;
    }

    // Resetting init and input files
    fseek(initfile, 0, SEEK_SET);
    fseek(inputfile, 0, SEEK_SET);


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
                int sp = getSP(va);
                int w = getW(va);
                int hit = 0;

                for(int i = 0; i < 4; ++i){
                    if(tlb[i][1] == sp && tlb[i][2] != 0){
                        int pa = tlb[i][2] + w;
                        for(int j = 0; j < 4; ++j) {
                            if (tlb[j][0] > tlb[i][0]) {
                                --tlb[j][0];
                            }
                        }
                        tlb[i][0] = 3;
                        hit = 1;
                        fprintf(outfile2, "h %d ", pa);
                    }
                }

                if(hit){
                    continue;
                }

                int s = getS(va);
                int p = getP(va);

                if(op == READ){
                    if(pmem[s] == -1 || pmem[pmem[s] + p] == -1){
                        fputs("m pf ", outfile2);
                    } else if (pmem[s] == 0 || pmem[pmem[s] + p] == 0){
                        fputs("m err ", outfile2);
                    } else {
                        for (int i = 0; i < 4; ++i) {
                            if (tlb[i][0] == 0) {
                                tlb[i][0] = 3;
                                tlb[i][1] = sp;
                                tlb[i][2] = pmem[pmem[s] + p];
                            } else {
                                --tlb[i][0];
                            }
                        }
                        fprintf(outfile2, "m %d ", pmem[pmem[s] + p] + w);
                    }
                } else if (op == WRITE){
                    if(pmem[s] == -1 || pmem[pmem[s] + p] == -1){
                        fputs("m pf ", outfile);
                    } else if (pmem[s] == 0) {
                        int new_pt = findEmptyPT(bitmap);
                        if(new_pt > 0){
                            int pt_addr = new_pt * FRAMESIZE;
                            setBit1(pt_addr, bitmap, masks);
                            setBit1(pt_addr + FRAMESIZE, bitmap, masks);
                            pmem[s] = pt_addr;
                            int new_pg = findEmptyFrame(1, bitmap);
                            if(new_pg > 0){
                                int pg_addr = new_pg * FRAMESIZE;
                                setBit1(pg_addr, bitmap, masks);
                                pmem[pmem[s] + p] = pg_addr;

                                for (int i = 0; i < 4; ++i) {
                                    if (tlb[i][0] == 0) {
                                        tlb[i][0] = 3;
                                        tlb[i][1] = sp;
                                        tlb[i][2] = pmem[pmem[s] + p];
                                    } else {
                                        --tlb[i][0];
                                    }
                                }
                                fprintf(outfile2, "m %d ", pmem[pmem[s] + p] + w);
                            }
                        }
                    } else if (pmem[pmem[s] + p] == 0){
                        int new_pg = findEmptyFrame(1, bitmap);
                        if(new_pg > 0){
                            int pg_addr = new_pg * FRAMESIZE;
                            setBit1(pg_addr, bitmap, masks);
                            pmem[pmem[s] + p] = pg_addr;

                            for (int i = 0; i < 4; ++i) {
                                if (tlb[i][0] == 0) {
                                    tlb[i][0] = 3;
                                    tlb[i][1] = sp;
                                    tlb[i][2] = pmem[pmem[s] + p];
                                } else {
                                    --tlb[i][0];
                                }
                            }
                            fprintf(outfile2, "m %d ", pmem[pmem[s] + p] + w);
                        }
                    } else {

                        for (int i = 0; i < 4; ++i) {
                            if (tlb[i][0] == 0) {
                                tlb[i][0] = 3;
                                tlb[i][1] = sp;
                                tlb[i][2] = pmem[pmem[s] + p];
                            } else {
                                --tlb[i][0];
                            }
                        }
                        fprintf(outfile2, "m %d ", pmem[pmem[s] + p] + w);
                    }
                }
            }
        }
    }
    //printBitmap(bitmap);
    //printMemory(2000, 3000, pmem);
    //printTLB(tlb);

    free(input);
    free(masks);
    free(bitmap);
    free(pmem);
}
