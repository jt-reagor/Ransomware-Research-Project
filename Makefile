CFLAGS = -Wall -O1
CC = gcc

malware: malware.o support.o
	$(CC) $(CFLAGS) -o malware malware.o support.o

malware.o: malware.c support.h
	$(CC) $(CFLAGS) -c -ggdb malware.c
	
support.o: support.c support.h
	$(CC) $(CFLAGS) -c support.c

clean:
	-rm -f *.o malware
