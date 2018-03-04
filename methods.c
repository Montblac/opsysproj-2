//
// Created by Sam Leyva on 2/26/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include "macros.h"


// Utility

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
    bitmap[frame] = bitmap[frame] & mask2;
}

// Translation
int getS(int addr){
    return (addr & S_BITS) >> 19;
}
int getP(int addr){
    return (addr & P_BITS) >> 9;
}
int getW(int addr){
    return addr & W_BITS;
}

// Search
int findEmptyFrame(const int * bitmap){
    for(int i = 1; i < MAPSIZE; ++i){
        if(bitmap[i] == 0){
            return i;
        }
    }
    return -1;
}

// Debugging
void printBitmap(int * bm){
    for(int i = 0; i < MAPSIZE; ++i){
        printf("%d ", bm[i]);
    }
    printf("\n");
}
void printMemory(int start, int end, int * pmem){
    for(int i = start; i < end; ++i){
        printf("%d ", pmem[i]);
    }
    printf("\n");
}