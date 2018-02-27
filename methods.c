//
// Created by Sam Leyva on 2/26/18.
//
#include <stdio.h>
#include <stdlib.h>

int * createBitMap(){
    return calloc(32, sizeof(int));
}
unsigned int * createBitMasks(){
    unsigned int * masks = malloc(32 * sizeof(unsigned int));
    masks[31] = 1;
    for(int i = 30; i > 0; --i){
        masks[i] = masks[i+1] << 1;
    }
    return masks;
}
