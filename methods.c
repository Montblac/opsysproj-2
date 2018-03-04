//
// Created by Sam Leyva on 2/26/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include "macros.h"

// Initialization
int * createBitMap(){
    return calloc(MAPSIZE, sizeof(int));
}
int * createBitMasks(){
    int * masks = malloc(MAPSIZE * sizeof(int));
    masks[MAPSIZE - 1] = 1;
    for(int i = 30; i >= 0; --i){
        masks[i] = masks[i+1] << 1;
    }
    return masks;
}
int * createPhysicalMem(){
    return calloc(MEMSIZE, sizeof(int));
}

// Accessors
int getFrame(int addr){
    return addr / FRAMESIZE;
}

// Mutator
void setBit1(int addr, int * bitmap, const int * masks){
    int frame = addr / FRAMESIZE;
    int index = addr % FRAMESIZE;
    bitmap[frame] = bitmap[frame] | masks[index];
}
void setBit0(int addr, int * bitmap, const int * masks){
    int frame = addr / FRAMESIZE;
    int index = addr % FRAMESIZE;
    int mask2 = ~masks[index];
    bitmap[frame] = bitmap[frame] && mask2;
}


// Input-checking
int isWord(const char * input){
    if(input == NULL) {
        return 0;
    }
    for (int i = 0; i < strlen(input); ++i) {
        if (!isalpha(input[i])) {
            return 0;
        }
    }
    return 1;
}
int isNumber(const char * input){
    if(input == NULL){
        return 0;
    }
    for(int i = 0; i < strlen(input); ++i){
        if(!isdigit(input[i])){
            return 0;
        }
    }
    return 1;
}
int isValidName(const char * name){
    if(name == NULL){return 0;}
    int size = 0;
    while(name[size] != NULL){
        ++size;
    }
    return size == 1 ? 1 : 0;
}
