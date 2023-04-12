#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "support.h"

void encrypt(unsigned int *ptr, unsigned int keyseedx, int chunks)
{
    // basic encryption algorithm
    // CBC with basic block-cipher
    // block cipher

    printf("ENCRYPTING...\n");
    for (int i = 0; i < chunks; i++)
    {
        // cipher block chaining
        if (i != 0)
            *ptr ^= *(ptr - 1);
        // swap nibbles
        unsigned int mask1 = 0xf0f0f0f0 & *ptr;
        unsigned int mask2 = 0x0f0f0f0f & *ptr;
        mask1 = mask1 >> 4;
        mask2 = mask2 << 4;
        *ptr = mask1 ^ mask2;
        // xor keyseed
        *ptr ^= keyseedx;
        ptr++;
    }
}

void decrypt(char *infilename)
{
    FILE *infile = fopen(infilename, "r");
    fseek(infile, 0, SEEK_END);
    int flen = ftell(infile);
    fseek(infile, 0, 0);

    // open file and start decryption
    char keyseedc[2 * KEY_LEN + 1];
    FILE *keyseedf;
    if (!(keyseedf = fopen("./keyseed", "r")))
    {
        printf("Error: Couldn't open keyseed\n");
        exit(8);
    }
    fgets(keyseedc, 2 * KEY_LEN + 1, keyseedf);
    unsigned int keyseedx;
    sscanf(keyseedc, "%x", &keyseedx);
    fclose(keyseedf);

    unsigned int chunks = flen / KEY_LEN;

    char buffer[flen + 1];
    fread(buffer, 1, flen, infile);
    buffer[flen] = '\0';

    printf("DECRYPTING...\n");
    // this reverses the encryption
    unsigned int *ptr = &buffer[flen - KEY_LEN];
    for (int i = chunks - 1; i >= 0; i--)
    {
        *ptr ^= keyseedx; // xor in keyseed
        unsigned int mask1 = 0xf0f0f0f0 & *ptr;
        unsigned int mask2 = 0x0f0f0f0f & *ptr;
        mask1 = mask1 >> 4;
        mask2 = mask2 << 4;
        *ptr = mask1 ^ mask2;
        if (i != 0) // cipher block chaining
            *ptr ^= *(ptr - 1);
        ptr--;
    }

    FILE *overwrite = fopen(infilename, "w");
    fwrite(buffer, 1, sizeof(buffer) - 1, overwrite);

    fclose(overwrite);
}
