#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "support.c"

int main(int argc, char *argv[])
{
    char *infile;
    if (argc == 2)
    {
        infile = argv[1];
    }
    else
    {
        printf("Usage: %s [<input_file>]\n", argv[0]);
    }

    printf("Hello, and welcome to the world's GREATEST DECRYPTION SYSTEM\n");

    decrypt(infile);
    return 0;
}