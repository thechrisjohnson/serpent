#include <limits.h>
#include <stdio.h>
#include "serpent.h"

static unsigned int sboxint(unsigned int input, int sbox) {
    unsigned int temp = 0x0;
    int i;
    for (i = 0; i < 32; i+= 4) {
        temp |= sboxes[sbox][((input << i) & fourBitMask) >> 28] >> i;
    }
    return temp;
}

void printbits(int input) {
    int i;
    int mult = 1;
    for (i = 0; i < 32; i++) {
        if (input & i) {
            printf("1");
        } else {
            printf("0");
        }
        mult *= 2;
    }

}

static unsigned int rotateleft(unsigned int input, int rotation) {
    return (input << rotation) | (input >> (sizeof(input)*CHAR_BIT - rotation));
    
}

int main() {

    unsigned int phi = 0x9e3779b9;

    unsigned int new = rotateleft(phi, 11);

    printf("w[0]: %X\n", new);

    unsigned int second = sboxint(new, 3);

    printf("k[0]: %X\n", second);

    
    return 0;
}
