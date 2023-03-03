#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "support.h"

extern FILE *infile;

void encrypt(char *input)
{
    FILE *toEncrypt;
    if (!(toEncrypt = fopen("./test", "r")))
    {
        printf("Error: Couldn't open %s\n", input);
        exit(8);
    }
    fseek(toEncrypt, 0, SEEK_END);
    int flen = ftell(toEncrypt);
    printf("Size: %d\n", flen);
    fseek(toEncrypt, 0, 0);
    // char line[MAX_LINE];
    // fgets(line, MAX_LINE, toEncrypt);
    // printf("%s", line);

    // make new file and start encryption
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
    printf("Keyseed: %x\n", keyseedx);

    // basic encryption algorithm
    // xors key in then xors by the last chunk of ciphertext

    unsigned int chunks = (flen + KEY_LEN - 1) / KEY_LEN; // super cool, super fast ceiling division...
    unsigned int pad = KEY_LEN - flen % KEY_LEN;
    printf("Splitting into %d chunks with a pad of %d...\n", chunks, pad);

    char buffer[flen + 1];
    fread(buffer, 1, flen, toEncrypt);
    buffer[flen] = '\0';
    printf("Contained in buffer:\n%s\n-------------------------\n", buffer);

    printf("ENCRYPTING...\n");
    unsigned int *ptr = &buffer[0];
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

    printf("BUFFER: \n%s\n", buffer);

    printf("DECRYPTING...\n");
    // this reverses the encryption
    ptr--;
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

    printf("BUFFER: \n%s\n", buffer);

    fclose(toEncrypt);
}
