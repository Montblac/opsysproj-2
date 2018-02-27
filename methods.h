//
// Created by Sam Leyva on 2/26/18.
//

#ifndef CS143B_PROJECT2_METHODS_H
#define CS143B_PROJECT2_METHODS_H

// Initialization
int * createBitMap();
unsigned int * createBitMasks();
int * createPhysicalMem();

// Input-checking;
int isWord(const char * input);
int isNumber(const char * input);
int isValidName(const char * name);

#endif //CS143B_PROJECT2_METHODS_H
