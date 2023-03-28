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
        // printf("Chunk %d\n", i);
        // printf("%p: %x\n", ptr, *ptr);

        if (i != 0) // cipher block chaining
            *ptr ^= *(ptr - 1);
        unsigned int mask04 = *ptr & 15;                                               // grab bits 0-4
        *ptr &= ~15;                                                                   // delete 0-4
        unsigned int mask812 = (*ptr & (15 << 8)) >> 8;                                // grab bits 8-12
        *ptr &= ~(15 << 8);                                                            // delete 8-12
        unsigned int mask1620 = (*ptr & (15 << 16)) >> 16;                             // grab bits 16-20
        *ptr &= ~(15 << 16);                                                           // delete 16-20
        unsigned int mask2428 = (*ptr & (15 << 24)) >> 24;                             // grab bits 24-28
        *ptr &= ~(15 << 24);                                                           // delete 24-28
        *ptr = *ptr >> 4;                                                              // shift right 4 bits
        *ptr ^= (mask04 << 4) ^ (mask812 << 12) ^ (mask1620 << 20) ^ (mask2428 << 28); // xor back in masks
        *ptr ^= keyseedx;                                                              // xor in keyseed

        // printf("%p: %x\n", ptr, *ptr);

        ptr++;
    }

    // printf("BUFFER: \n%s\n", buffer);
}

void decrypt(char *infilename)
{
    FILE *infile = fopen(infilename, "r");
    fseek(infile, 0, SEEK_END);
    int flen = ftell(infile);
    // printf("Size: %d\n", flen);
    fseek(infile, 0, 0);

    // open file and start decryption
    char keyseedc[2 * KEY_LEN + 1];
    FILE *keyseedf;
    if (!(keyseedf = fopen("./keyseed", "r")))
    {
        printf("Error: Couldn't open keyseed\n");
        exit(8);
    }
    int ret = fgets(keyseedc, 2 * KEY_LEN + 1, keyseedf);
    unsigned int keyseedx;
    sscanf(keyseedc, "%x", &keyseedx);
    fclose(keyseedf);
    // printf("Keyseed: %x\n", keyseedx);

    unsigned int chunks = flen / KEY_LEN;

    char buffer[flen + 1];
    ret = fread(buffer, 1, flen, infile);
    buffer[flen] = '\0';
    // printf("Contained in buffer:\n%s\n-------------------------\n", buffer);

    printf("DECRYPTING...\n");
    // this reverses the encryption
    unsigned int *ptr = &buffer[flen - KEY_LEN];
    for (int i = chunks - 1; i >= 0; i--)
    {
        // printf("Chunk %d\n", i);
        // printf("%p: %x\n", ptr, *ptr);

        *ptr ^= keyseedx;                                                              // xor in keyseed
        unsigned int mask04 = *ptr & 15;                                               // grab bits 0-4
        *ptr &= ~15;                                                                   // delete 0-4
        unsigned int mask812 = (*ptr & (15 << 8)) >> 8;                                // grab bits 8-12
        *ptr &= ~(15 << 8);                                                            // delete 8-12
        unsigned int mask1620 = (*ptr & (15 << 16)) >> 16;                             // grab bits 16-20
        *ptr &= ~(15 << 16);                                                           // delete 16-20
        unsigned int mask2428 = (*ptr & (15 << 24)) >> 24;                             // grab bits 24-28
        *ptr &= ~(15 << 24);                                                           // delete 24-28
        *ptr = *ptr >> 4;                                                              // shift right 4 bits
        *ptr ^= (mask04 << 4) ^ (mask812 << 12) ^ (mask1620 << 20) ^ (mask2428 << 28); // xor back in masks
        if (i != 0)                                                                    // cipher block chaining
            *ptr ^= *(ptr - 1);
        // printf("%p: %x\n", ptr, *ptr);
        ptr--;
    }

    // printf("BUFFER: \n%s\n", buffer);

    FILE *overwrite = fopen(infilename, "w");
    fwrite(buffer, 1, sizeof(buffer) - 1, overwrite);

    fclose(overwrite);
}