// function prototypes
extern void encrypt(unsigned int *ptr, unsigned int keyseedx, int chunks);
extern void decrypt(char *infilename);

#define MAX_LINE 100
#define KEY_LEN 4