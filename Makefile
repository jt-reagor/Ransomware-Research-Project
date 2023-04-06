CFLAGS = -Wall -O0
CC = gcc

malware: malware.o support.o
	$(CC) $(CFLAGS) -o malware malware.o support.o

malware.o: malware.c support.h
	$(CC) $(CFLAGS) -c -ggdb malware.c

decrypter: decrypter.o support.o
	$(CC) $(CFLAGS) -o testenv/decrypter decrypter.o support.o

decrypter.o: decrypter.c support.h
	$(CC) $(CFLAGS) -c -ggdb decrypter.c

support.o: support.c support.h
	$(CC) $(CFLAGS) -c support.c

clean:
	-rm -rf *.o malware

loadenv:
	-cp supersecretfile.txt ./testenv/supersecretfile.txt
	-cp secretfile2.txt ./testenv/secretfile2.txt
	-cp keyseed ./testenv/keyseed
