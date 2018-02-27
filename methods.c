//
// Created by Sam Leyva on 2/26/18.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#define MEMSIZE 524288

// Initialization
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
int * createPhysicalMem(){
    return calloc(MEMSIZE, sizeof(int));
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
