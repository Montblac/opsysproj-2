//
// Created by Sam Leyva on 2/26/18.
//

#ifndef CS143B_PROJECT2_METHODS_H
#define CS143B_PROJECT2_METHODS_H

// Utility

// Initialization
int * createBitMap();
int * createBitMasks();
int * createPhysicalMem();

// Mutator
void setBit1(int addr, int * bitmap, const int * masks);
void setBit0(int addr, int * bitmap, const int * masks);

// Translation
int getS(int addr);
int getP(int addr);
int getW(int addr);

// Search
int findEmptyFrame(const int * bitmap);

// Debugging
void printBitmap(int * bm);
void printMemory(int start, int end, int * pmem);

#endif //CS143B_PROJECT2_METHODS_H
