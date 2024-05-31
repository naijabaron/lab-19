#ifndef LAB_19_LAB_19_H
#define LAB_19_LAB_19_H

#include <stdio.h>

void transposeMatrices(FILE* input, FILE* output);

void trimFloats(FILE* input, FILE* output);

void solveSentence(FILE* file);

void filterWordsByContent(FILE* input, FILE* output, char* content);

void filterOnlyLongestWords(FILE* input, FILE* output);

void removeSentencesWithRoot(FILE* input, FILE* output, int root);

void sortNumbersByPositivity(FILE* input, FILE* output);

void transposeNonSymmetricalMatrices(FILE* input, FILE* output);

void collectTheBestTeam(FILE* input, FILE* output, int teamSize);

void filterProducts(FILE* productsFile, FILE* ordersFile);

#endif //LAB_19_LAB_19_H