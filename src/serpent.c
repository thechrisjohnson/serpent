#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "serpent.h"

static unsigned int rotateleft(unsigned int input, int rotation) {
    return (input << rotation) | (input >> (sizeof(input)*CHAR_BIT - rotation));
    
}

static block *permutation(block* input, int* permutation) {
    printf("Pre-Permute: 0x%08lX%08lX%08lX%08lX\n", input->first, input->second, input->third, input->fourth);
    block* permuted = malloc(sizeof(block));
    permuted->first = 0x0;
    permuted->second = 0x0;
    permuted->third = 0x0;
    permuted->fourth = 0x0;

    int i;
    for (i = 0; i < 32; i++) {
        unsigned long t = (input->first << i) & mask;
        if (t) {
            int location = permutation[i];
            if (location > 95) {
                location -= 96;
                permuted->fourth = permuted->fourth | (mask >> location);
            } else if (location > 63) {
                location -= 64;
                permuted->third = permuted->third | (mask >> location);
            } else if (location > 31) {
                location -= 32;
                permuted->second = permuted->second | (mask >> location);
            } else {
                permuted->first = permuted->first | (mask >> location);
            }
        }
    }

    for (i = 0; i < 32; i++) {
        unsigned long t = (input->second << i) & mask;
        if (t) {
            int location = permutation[i+32];
            if (location > 95) {
                location -= 96;
                permuted->fourth = permuted->fourth | (mask >> location);
            } else if (location > 63) {
                location -= 64;
                permuted->third = permuted->third | (mask >> location);
            } else if (location > 31) {
                location -= 32;
                permuted->second = permuted->second | (mask >> location);
            } else {
                permuted->first = permuted->first | (mask >> location);
            }
        }
    }

    for (i = 0; i < 32; i++) {
        unsigned long t = (input->third << i) & mask;
        if (t) {
            int location = permutation[i+64];
            if (location > 95) {
                location -= 96;
                permuted->fourth = permuted->fourth | (mask >> location);
            } else if (location > 63) {
                location -= 64;
                permuted->third = permuted->third | (mask >> location);
            } else if (location > 31) {
                location -= 32;
                permuted->second = permuted->second | (mask >> location);
            } else {
                permuted->first = permuted->first | (mask >> location);
            }
        }
    }

    for (i = 0; i < 32; i++) {
        unsigned long t = (input->fourth << i) & mask;
        if (t) {
            int location = permutation[i+96];
            if (location > 95) {
                location -= 96;
                permuted->fourth = permuted->fourth | (mask >> location);
            } else if (location > 63) {
                location -= 64;
                permuted->third = permuted->third | (mask >> location);
            } else if (location > 31) {
                location -= 32;
                permuted->second = permuted->second | (mask >> location);
            } else {
                permuted->first = permuted->first | (mask >> location);
            }
        }
    }
    printf("Permute: 0x%08lX%08lX%08lX%08lX\n", permuted->first, permuted->second, permuted->third, permuted->fourth);
    return permuted;
}

static unsigned int sboxint(unsigned int input, int sbox) {
    unsigned int temp = 0x0;
    int i;
    for (i = 0; i < 32; i+= 4) {
        temp |= sboxes[sbox][((input << i) & fourBitMask) >> 28] >> i;
    }
    return temp;
}

static block *initialpermutation(block* input) {
    return permutation(input, initPerm);
}

static block *finalpermutation(block* input) {
    return permutation(input, finalPerm);
}

static block **generatekeys(block* input) {
    unsigned int w[140];
    unsigned int k[132];
    int sbox = 3;
    int i;
    
    // Move pre-key into array
    w[0] = input->first;
    w[1] = input->second;
    w[2] = input->third;
    w[3] = input->fourth;
    w[4] = (unsigned int) 0x0000000U;
    w[5] = (unsigned int) 0x0U;
    w[6] = (unsigned int) 0x0U;
    w[7] = (unsigned int) 0x0U;

    for (i = 0; i < 8; i++) {
        printf("w[%d]: 0x%08lX\n", i-8, w[i]);
    }

    //Generate 132 more words
    for (i = 0; i < 132; i++) {
        w[i+8] = rotateleft(w[i] ^ w[i+3] ^ w[i+5] ^ w[i+7] ^ phi ^ i, 11);
        printf("w[%d]: 0x%08lX\n", i, w[i+8]);
    }

    //Pass words through S-boxes
    for (i = 0; i < numrounds; i++) {
        sbox = (numrounds + 3 - i) % numrounds;
        k[i] = 0x0;
        k[i+33] = 0x0;
        k[i+66] = 0x0;
        k[i+99] = 0x0;
        unsigned int first = w[i+8];
        unsigned int second = w[i+42];
        unsigned int third = w[i+76];
        unsigned int fourth = w[i+110];
        
        int j;
        for (j = 0; j < 32, j++) {
            unsigned int a = (first<<j+3) & mask;
            unsigned int b = (second<<j+3) & mask;
            unsigned int c = (third<<j+3) & mask;
            unsigned int d = (fourth<<j+3) & mask;

        }

        temp |= (first>>3) | (second >> 2) | (third >> 1) | fourth;

        temp = sboxint(temp, sbox);
        
        first = (temp << 3) & mask;
        second = (temp << 2) & mask;
        third = (temp << 1) & mask;
        fourth = temp & mask;

        k[i] |= first >> j;
        k[i+1] |= second >> j;
        k[i+2] |= third >> j;
        k[i+3] |= fourth >> j;
        

        //printf("k[%d]: 0x%08lX\n", i, k[i]);
        //printf("k[%d]: 0x%08lX\n", i+1, k[i+1]);
        //printf("k[%d]: 0x%08lX\n", i+2, k[i+2]);
        //printf("k[%d]: 0x%08lX\n", i+3, k[i+3]);
    }

    //subkey Ki = k4i, k4i+1, k4i+2, and k4i+3
    block presubkeys[numkeys];

    for (i = 0; i < numkeys; i++) {
        presubkeys[i].first = k[i*4];
        presubkeys[i].second = k[i*4+1];
        presubkeys[i].third = k[i*4+2];
        presubkeys[i].fourth = k[i*4+3];
        printf("K[%d]: 0x%08lX%08lX%08lX%08lX\n", i, presubkeys[i].first, presubkeys[i].second, presubkeys[i].third, presubkeys[i].fourth);
    }

    //Permute subkeys
    block** subkeys = malloc(sizeof(block*) * 32);
    for (i = 0; i < numkeys; i++) {
        subkeys[i] = initialpermutation(&presubkeys[i]);
    }

    return subkeys;
}

static block *encrypt(block* text, block* key) {
    //Generate round subkeys
    block** serpentkey = generatekeys(key);

    block* roundInput = initialpermutation(text);

    int i;
    for(i = 0; i < numrounds; i++){
        //XOR round subkey
        unsigned int first = roundInput->first ^ serpentkey[i]->first;
        unsigned int second = roundInput->second ^ serpentkey[i]->second;
        unsigned int third = roundInput->third ^ serpentkey[i]->third;
        unsigned int fourth = roundInput->fourth ^ serpentkey[i]->fourth;

        printf("Round[%d]: 0x%08lX%08lX%08lX%08lX\n", i, first, second, third, fourth);

        //Apply S boxes
        first = sboxint(first, i%8);
        second = sboxint(second, i%8);
        third = sboxint(third, i%8);
        fourth = sboxint(fourth, i%8);

        if (i < 31) {
            //Perform linear transformation
            first = rotateleft(first, 13);
            third = rotateleft(third, 3);
            second = second ^ first ^ third;
            fourth = fourth ^ third ^ (first << 3);
            second = rotateleft(second, 1);
            fourth = rotateleft(fourth, 7);
            first = first ^ second ^ fourth;
            third = third ^ fourth ^ (second << 7);
            first = rotateleft(first, 5);
            third = rotateleft(third, 22);

        } else {
            // XOR last key
            first ^= serpentkey[i+1]->first;
            second ^= serpentkey[i+1]->second;
            third ^= serpentkey[i+1]->third;
            fourth ^= serpentkey[i+1]->fourth;
        }

        roundInput->first = first;
        roundInput->second = second;
        roundInput->third = third;
        roundInput->fourth = fourth;
    }

    printf("Final: 0x%08lX%08lX%08lX%08lX\n", roundInput->first, roundInput->second, roundInput->third, roundInput->fourth);

    //Clean up memory
    for (i = 0; i < numkeys; i++) {
        free(serpentkey[i]);
    }
    free(serpentkey);
                
    //Finish with final permutation
    return finalpermutation(roundInput);
}

int main(int argc, char** argv) {
    block* text = malloc(sizeof(block));
    text->first = 0x0U;
    text->second = 0U;
    text->third = 0U;
    text->fourth = 0U;
    block* key = malloc(sizeof(block));
    key->first = 0x0000000;
    key->second = 0U;
    key->third = 0U;
    key->fourth = 0U;
    
    block* cipher = encrypt(text, key);

    printf("Key: 0x%08lX%08lX%08lX%08lX\n", key->first, key->second, key->third, key->fourth);
    printf("Text: 0x%08lX%08lX%08lX%08lX\n", text->first, text->second, text->third, text->fourth);
    printf("Cipher: 0x%08lX%08lX%08lX%08lX\n", cipher->first, cipher->second, cipher->third, cipher->fourth);

    return 0;
}
