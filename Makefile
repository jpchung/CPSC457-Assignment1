CC=gcc
CFLAG=-Wall
LIBS=-lcrypto -lssl

all: gls

gls: gls.c
	$(CC) $(CFLAGS) $(LIBS) $< -o $@ 

clean:
	rm -f gls
