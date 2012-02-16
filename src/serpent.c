#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "serpent.h"

unsigned char getBitFromWord(unsigned int x, int p) {
  return (unsigned char) ((x & ((unsigned int) 0x1 << p)) >> p);
}

unsigned char getBitFromNibble(unsigned char x, int p) {
  return (unsigned char) ((x & (0x1 << p)) >> p);
}

unsigned char makeNibble(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) {
  /* Return a nibble made of the given bits (b0 being the least significant). */
  return (unsigned char) (b0 | (b1 << 1) | (b2 << 2) | (b3 << 3));
}

unsigned char getNibble(unsigned int x, int p) {
  return (unsigned char) (0xf & (x >> (p*4)));
}

static unsigned int rotateleft(unsigned int input, int rotation) {
    return (input << rotation) | (input >> (sizeof(input)*CHAR_BIT - rotation));
}

static unsigned int rotateright(unsigned int input, int rotation) {
    return (input >> rotation) | (input << (sizeof(input)*CHAR_BIT - rotation));
}

static block *permutation(block* input, int* permutation) {
    //printf("Pre-Permute: 0x%08lX%08lX%08lX%08lX\n", input->first, input->second, input->third, input->fourth);
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
    //printf("Permute: 0x%08lX%08lX%08lX%08lX\n", permuted->first, permuted->second, permuted->third, permuted->fourth);
    return permuted;
}

static unsigned char sboxchar(unsigned char input, int sbox) {
    return sboxes[sbox][input];
}

static unsigned int sboxint(unsigned int input, int sbox) {
    unsigned int temp = 0x0;
    int i;
    for (i = 0; i < 8; i++) {
      temp |= ((unsigned int) sboxchar(getNibble(input, i), sbox)) << (i*4);
    }
    return temp;
}

static unsigned char invsboxchar(unsigned char input, int sbox) {
    return invsboxes[sbox][input];
}

static unsigned int invsboxint(unsigned int input, int sbox) {
    unsigned int temp = 0x0;
    int i;
    for (i = 0; i < 8; i++) {
      temp |= ((unsigned int) invsboxchar(getNibble(input, i), sbox)) << (i*4);
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
    int i,j,l;
    
    // Move pre-key into array
    w[0] = input->first;
    w[1] = input->second;
    w[2] = input->third;
    w[3] = input->fourth;
    w[4] = (unsigned int) 0x80000000U;
    w[5] = (unsigned int) 0x0U;
    w[6] = (unsigned int) 0x0U;
    w[7] = (unsigned int) 0x0U;

    /*for (i = 0; i < 8; i++) {
        printf("w[%d]: 0x%08lX\n", i-8, w[i]);
    }*/

    //Generate 132 more words
    for (i = 0; i < 132; i++) {
        w[i+8] = rotateleft(w[i] ^ w[i+3] ^ w[i+5] ^ w[i+7] ^ phi ^ i, 11);
        //printf("w[%d]: 0x%08lX\n", i, w[i+8]);
    }

    for (i = 0; i < numrounds+1; i++) {
        k[0+4*i] = k[1+4*i] = k[2+4*i] = k[3+4*i] = 0;
        for (j = 0; j < 32; j++) {
          unsigned char input = makeNibble(getBitFromWord(w[8+4*i], j),
                             getBitFromWord(w[9+4*i], j),
                             getBitFromWord(w[10+4*i], j),
                             getBitFromWord(w[11+4*i], j));
          unsigned char output = sboxchar(input, sbox);
          for (l = 0; l < 4; l++) {
           k[l+4*i] |= ((unsigned char) getBitFromNibble(output, l)) << j;
          }
        }
        sbox--;
        if (sbox < 0) {
            sbox = 7;
        }
    }

    /*for (i = 0; i < 132; i++) {
        printf("k[%d]: %x\n", i, k[i]);
    }*/
    //subkey Ki = k4i, k4i+1, k4i+2, and k4i+3
    block presubkeys[numkeys];

    for (i = 0; i < numkeys; i++) {
        presubkeys[i].first = k[i*4];
        presubkeys[i].second = k[i*4+1];
        presubkeys[i].third = k[i*4+2];
        presubkeys[i].fourth = k[i*4+3];
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

    //Clean up memory
    for (i = 0; i < numkeys; i++) {
        free(serpentkey[i]);
    }
    free(serpentkey);
                
    //Finish with final permutation
    return finalpermutation(roundInput);
}

static block *decrypt(block* cipher, block* key) {
    //Generate round subkeys
    block** serpentkey = generatekeys(key);

    block* roundInput = initialpermutation(cipher);

    int i;
    for(i = numrounds-1; i >= 0; i--){
        unsigned int first = roundInput->first;
        unsigned int second = roundInput->second;
        unsigned int third = roundInput->third;
        unsigned int fourth = roundInput->fourth;
        if (i < 31) {
            //Perform reverse linear transformation
            third = rotateright(third, 22);
            first = rotateright(first, 5);
            third = third ^ fourth ^ (second << 7);
            first = first ^ second ^ fourth;
            fourth = rotateright(fourth, 7);
            second = rotateright(second, 1);
            fourth = fourth ^ third ^ (first << 3);
            second = second ^ first ^ third;
            third = rotateright(third, 3);
            first = rotateright(first, 13);
        } else {
            // XOR last key
            first ^= serpentkey[i+1]->first;
            second ^= serpentkey[i+1]->second;
            third ^= serpentkey[i+1]->third;
            fourth ^= serpentkey[i+1]->fourth;
        }

        //Apply S boxes
        first = invsboxint(first, i%8);
        second = invsboxint(second, i%8);
        third = invsboxint(third, i%8);
        fourth = invsboxint(fourth, i%8);

        //XOR round subkey
        first = first ^ serpentkey[i]->first;
        second = second ^ serpentkey[i]->second;
        third = third ^ serpentkey[i]->third;
        fourth = fourth ^ serpentkey[i]->fourth;

        roundInput->first = first;
        roundInput->second = second;
        roundInput->third = third;
        roundInput->fourth = fourth;
    }

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
    text->first = 0xFFFFFFFF;
    text->second = 0xFFFFFFFF;
    text->third = 0xFFFFFFFF;
    text->fourth = 0xFFFFFFFF;
    block* key = malloc(sizeof(block));
    key->first = 0xFFFFFFFF;
    key->second = 0xFFFFFFFF;
    key->third = 0xFFFFFFFF;
    key->fourth = 0xFFFFFFFF;
    
    block* cipher = encrypt(text, key);

    printf("Key: 0x%08lX%08lX%08lX%08lX\n", key->first, key->second, key->third, key->fourth);
    printf("Text: 0x%08lX%08lX%08lX%08lX\n", text->first, text->second, text->third, text->fourth);
    printf("Cipher: 0x%08lX%08lX%08lX%08lX\n", cipher->first, cipher->second, cipher->third, cipher->fourth);

    block* decipher = decrypt(cipher, key);

     
    printf("Clear: 0x%08lX%08lX%08lX%08lX\n", decipher->first, decipher->second, decipher->third, decipher->fourth);

    return 0;
}
