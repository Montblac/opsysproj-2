#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "methods.h"

int main(int argc, char * argv[]) {
    // Variable Declaration
    FILE * initfile;
    FILE * inputfile;

    size_t size;
    ssize_t inputsize;
    char * input = NULL;
    char * value1 = NULL;
    char * value2 = NULL;
    char * value3 = NULL;
    char * pathtemp;

    // Initialize bit map, bit masks, physical memory
    int * bitmap = createBitMap();
    unsigned int * masks = createBitMasks();
    int * pmem = createPhysicalMem();

    // Initialize page tables and pages
    printf("Init File Path: ");
    inputsize = getline(&input, &size, stdin);
    pathtemp = strtok(input, " \n");
    initfile = fopen(pathtemp, "r");
    if(initfile == NULL){
        printf("ERROR: Invalid initialization file path\n");
        return 0;
    } else {
        char * temp, * value;
        inputsize = getline(&input, &size, initfile);

        // Store starting address of page table f in segment s
        temp = strtok(input, " \n");
        if(temp == NULL) {
            printf("ERROR: Missing segment value\n");
            return 0;
        }
        value1 = strdup(temp);

        temp = strtok(NULL, " \n");
        if(temp == NULL){
            printf("ERROR: Missing page table address\n");
            return 0;
        }

        if(!isNumber(value1) || !isNumber(temp)){
            printf("ERROR: Invalid input; input are not integers");
            return 0;
        }
        pmem[strtol(value1, NULL, 10)] = strtol(temp, NULL, 10);




        free(value1);

    }


    // TEST INPUT
    // /Users/Sam/Desktop/test.txt



    free(masks);
    free(bitmap);
    fclose(initfile);
}