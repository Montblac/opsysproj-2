//
// Created by Sam Leyva on 2/26/18.
//

#ifndef CS143B_PROJECT2_METHODS_H
#define CS143B_PROJECT2_METHODS_H

// Initialization
int * createBitMap();
int * createBitMasks();
int * createPhysicalMem();

// Accessor
int getFrame(int addr);

// Mutator
void setBit1(int addr, int * bitmap, const int * masks);
void setBit0(int addr, int * bitmap, const int * masks);

// Input-checking;
int isWord(const char * input);
int isNumber(const char * input);
int isValidName(const char * name);

#endif //CS143B_PROJECT2_METHODS_H
