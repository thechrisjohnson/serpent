#include <limits.h>
#include <stdlib.h>
#include "serpent-orig.h"

unsigned char getbitint(unsigned int x, int p) {
    return (unsigned char) ((x & ((unsigned int) 0x1 << p)) >> p);
}

unsigned char getbitblock(block* input, int p) {
    int *get;

    if (p > 95) {
        get = &(input->first);
    } else if (p > 63) {
        get = &(input->second);
    } else if (p > 31) {
        get = &(input->third);
    } else {
        get = &(input->fourth);
    }

    return (unsigned char) ((*get & ((unsigned int) 0x1 << p%32)) >> p%32);

}

void setbitblock(block* input, int p, unsigned char v) {
    int *set;

    if (p > 95) {
        set = &(input->first);
    } else if (p > 63) {
        set = &(input->second);
    } else if (p > 31) {
        set = &(input->third);
    } else {
        set = &(input->fourth);
    }
       
    if (v) {
        *set |= ((unsigned int) 0x1 << p%32);
    } else {
        *set &= ~((unsigned int) 0x1 << p%32);
    }
}

unsigned char getbitfour(unsigned char x, int p) {
    return (unsigned char) ((x & (0x1 << p)) >> p);
}

unsigned char makechar(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3) {
    return (unsigned char) (b0 | (b1 << 1) | (b2 << 2) | (b3 << 3));
}

unsigned char getfourbits(unsigned int x, int p) {
    return (unsigned char) (0xf & (x >> (p*4)));
}

static unsigned int rotateleft(unsigned int input, int rotation) {
    return (input << rotation) | (input >> (sizeof(input)*CHAR_BIT - rotation));
}

static block *permutation(block* input, int* permutation) {
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
    return permuted;
}

static unsigned char sboxchar(unsigned char input, int sbox) {
    return sboxes[sbox][input];
}

static unsigned int sboxint(unsigned int input, int sbox) {
    unsigned int temp = 0x0;
    int i;
    for (i = 0; i < 8; i++) {
      temp |= ((unsigned int) sboxchar(getfourbits(input, i), sbox)) << (i*4);
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
      temp |= ((unsigned int) invsboxchar(getfourbits(input, i), sbox)) << (i*4);
    }
    return temp;
}

static block *initialpermutation(block* input) {
    return permutation(input, finalPerm);
}

static block *finalpermutation(block* input) {
    return permutation(input, initPerm);
}

static block *lintrans(block* input) {
    int i, j, t;
    unsigned char b;
    block* output = malloc(sizeof(block));
    output->first = 0x0;
    output->second = 0x0;
    output->third = 0x0;
    output->fourth = 0x0;

    for (i = 0; i < 128; i++) {
        b = 0;
        j = 0;
        while (lineartransform[i][j] != END) {
            b ^= getbitblock(input, lineartransform[i][j]);
            j++;
        }
        setbitblock(output, i, b);
    }

    return output;
} 

static block *invlintrans(block* input) {
    int i, j;
    unsigned char b;
    block* output = malloc(sizeof(block));
    output->first = 0x0;
    output->second = 0x0;
    output->third = 0x0;
    output->fourth = 0x0;

    for (i = 0; i < 128; i++) {
        b = 0;
        j = 0;
        while (inverselineartransform[i][j] != END) {
            b ^= getbitblock(input, inverselineartransform[i][j]);
            j++;
        }
        setbitblock(output, i, b);
    }
    return output;
} 

static block **generatekeys(key* input) {
    unsigned int w[140];
    unsigned int k[132];
    int sbox = 3;
    int i,j,l;
    
    // Move pre-key into array
    w[7] = input->first;
    w[6] = input->second;
    w[5] = input->third;
    w[4] = input->fourth;
    w[3] = input->fifth;
    w[2] = input->sixth;
    w[1] = input->seventh;
    w[0] = input->eighth;

    //Generate 132 more words
    for (i = 0; i < 132; i++) {
        w[i+8] = rotateleft(w[i] ^ w[i+3] ^ w[i+5] ^ w[i+7] ^ phi ^ i, 11);
    }

    for (i = 0; i < numrounds+1; i++) {
        k[0+4*i] = k[1+4*i] = k[2+4*i] = k[3+4*i] = 0;
        for (j = 0; j < 32; j++) {
          unsigned char input = makechar(getbitint(w[8+4*i], j),
                             getbitint(w[9+4*i], j),
                             getbitint(w[10+4*i], j),
                             getbitint(w[11+4*i], j));
          unsigned char output = sboxchar(input, sbox);
          for (l = 0; l < 4; l++) {
           k[l+4*i] |= ((unsigned char) getbitfour(output, l)) << j;
          }
        }
        sbox--;
        if (sbox < 0) {
            sbox = 7;
        }
    }

    //subkey Ki = k4i, k4i+1, k4i+2, and k4i+3
    block presubkeys[numkeys];

    for (i = 0; i < numkeys; i++) {
        presubkeys[i].fourth = k[i*4];
        presubkeys[i].third = k[i*4+1];
        presubkeys[i].second = k[i*4+2];
        presubkeys[i].first = k[i*4+3];
    }

    //Permute subkeys
    block** subkeys = malloc(sizeof(block*) * 32);
    for (i = 0; i < numkeys; i++) {
        subkeys[i] = initialpermutation(&presubkeys[i]);
    }

    return subkeys;
}

block *encrypt(block* text, key* keyoriginial) {
    //Generate round subkeys
    block** serpentkey = generatekeys(keyoriginial);

    block* roundInput = initialpermutation(text);

    int i;
    for(i = 0; i < numrounds; i++){
        //XOR round subkey
        roundInput->first ^= serpentkey[i]->first;
        roundInput->second ^= serpentkey[i]->second;
        roundInput->third ^= serpentkey[i]->third;
        roundInput->fourth ^= serpentkey[i]->fourth;

        //Apply S boxes
        roundInput->first = sboxint(roundInput->first, i%8);
        roundInput->second = sboxint(roundInput->second, i%8);
        roundInput->third = sboxint(roundInput->third, i%8);
        roundInput->fourth = sboxint(roundInput->fourth, i%8);
        
        if (i < 31) {
            //Perform linear transformation
            block* output = lintrans(roundInput);
            block* temp = roundInput;
            roundInput = output;
            free(temp);

        } else {
            // XOR last key
            roundInput->first ^= serpentkey[i+1]->first;
            roundInput->second ^= serpentkey[i+1]->second;
            roundInput->third ^= serpentkey[i+1]->third;
            roundInput->fourth ^= serpentkey[i+1]->fourth;
        }
    }

    //Clean up memory
    for (i = 0; i < numkeys; i++) {
        free(serpentkey[i]);
    }
    free(serpentkey);
                
    //Finish with final permutation
    return finalpermutation(roundInput);
}

block *decrypt(block* cipher, key* keyoriginal) {
    //Generate round subkeys
    block** serpentkey = generatekeys(keyoriginal);

    block* roundInput = initialpermutation(cipher);

    int i;
    for(i = numrounds-1; i >= 0; i--){
        if (i < 31) {
            //Perform reverse linear transformation
            block* output = invlintrans(roundInput);
            block* temp = roundInput;
            roundInput = output;
            free(temp);
        } else {
            // XOR last key
            roundInput->first ^= serpentkey[i+1]->first;
            roundInput->second ^= serpentkey[i+1]->second;
            roundInput->third ^= serpentkey[i+1]->third;
            roundInput->fourth ^= serpentkey[i+1]->fourth;
        }

        //Apply S boxes
        roundInput->first = invsboxint(roundInput->first, i%8);
        roundInput->second = invsboxint(roundInput->second, i%8);
        roundInput->third = invsboxint(roundInput->third, i%8);
        roundInput->fourth = invsboxint(roundInput->fourth, i%8);

        //XOR round subkey
        roundInput->first ^= serpentkey[i]->first;
        roundInput->second ^= serpentkey[i]->second;
        roundInput->third ^= serpentkey[i]->third;
        roundInput->fourth ^= serpentkey[i]->fourth;
    }

    //Clean up memory
    for (i = 0; i < numkeys; i++) {
        free(serpentkey[i]);
    }
    free(serpentkey);
                
    //Finish with final permutation
    return finalpermutation(roundInput);
}
