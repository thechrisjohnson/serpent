#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "serpent.h"

int main(int argc, char** argv) {
    int en;

    //Check args
    if (argc != 3) {
        printf("usage: serpent [-e|-d] key\n");
        return 1;
    }
    if (strcmp(argv[1], "-e") == 0) {
        en = 1;
    } else if(strcmp(argv[1], "-d") == 0) {
        en = 0;
    } else {
        printf("usage: serpent [-e|-d] key\n");
        return 1;
    }
    
    //Import key
    key* keyinput = malloc(sizeof(key));

    int keynum = sscanf(argv[2], "%8x%8x%8x%8x%8x%8x%8x%8x", &keyinput->first, &keyinput->second, &keyinput->third, 
        &keyinput->fourth, &keyinput->fifth, &keyinput->sixth, &keyinput->seventh, &keyinput->eighth);

    if (keynum != 8) {
        printf("Invalid key\n");
        return 1;
    }

    block* blockinput = malloc(sizeof(block));
    memset(blockinput, 0, sizeof(block)); 
    block* output;
    int in;
    //Encrypt or decrypt until finished
    while(in = fscanf(stdin, "%8x%8x%8x%8x", &blockinput->first, &blockinput->second, 
        &blockinput->third, &blockinput->fourth) != EOF) {
        
        if (en) {
            output = encrypt(blockinput, keyinput);
        } else {
            output = decrypt(blockinput, keyinput);
        }

        //Output
        printf("%08X%08X%08X%08X", output->first, output->second, output->third, output->fourth);
        free(output);
        memset(blockinput, 0, sizeof(block)); 
    }

    return 0;
}
