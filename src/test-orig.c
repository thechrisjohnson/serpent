#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serpent-orig.h"

#define TIMES 100000

void printClock(char* name, clock_t time) {
    float total = ((float) time) / CLOCKS_PER_SEC;
    float average = total / ((float) TIMES);

    printf("%s\t\t%f\t\t%f\n", name, total, average);
}

int main(int argc, char** argv) {
    int i;
    clock_t en, de;

    block* input = malloc(sizeof(block));
    key* k = malloc(sizeof(key));

    memset(input, 0, sizeof(block)); 
    memset(k, 0, sizeof(key)); 
    
    en = clock();
    //Encrypt
    for (i = 0; i < TIMES; i++) {
        free(encrypt(input, k));
    }
    en = clock() - en;

    //Decrypt
    de = clock();
    for (i = 0; i < TIMES; i++) {
        free(decrypt(input, k));
    }
    de = clock() - de;

    //Header
    printf("Running %d times each...\n", TIMES);
    printf("Name\t\tTotal Time(s)\t\tAve. Time(s)\n");

    //Print all 
    printClock("ENCRYPT", en);
    printClock("DECRYPT", de);
}
