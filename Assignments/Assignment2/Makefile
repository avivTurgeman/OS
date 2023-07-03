CC = gcc
CFLAGS = -c -Wall -Werror -fPIC
LDFLAGS = -shared -Wall -ldl
RM = rm -f


.PHONY: all clean cleantxt

all: PartA PartB PartC

PartA: cmp copy

PartB: codecA codecB encode decode

PartC: stshell

cmp: cmp.c
	$(CC) -Wall -g cmp.c -o cmp

copy: copy.c
	$(CC) -Wall -g copy.c -o copy

codecA.o: codecA.c
	$(CC) ${CFLAGS}  codecA.c -o codecA.o

codecA: codecA.o
	$(CC) ${LDFLAGS} codecA.o -o codecA

codecB.o: codecB.c
	$(CC) ${CFLAGS}  codecB.c -o codecB.o

codecB: codecB.o
	$(CC) ${LDFLAGS} codecB.o -o codecB

encode.o: encode.c
	$(CC) -c -Wall encode.c -o encode.o

encode: encode.o
	$(CC) -Wall -g encode.o -o encode

decode.o: decode.c
	$(CC) -c -Wall -g decode.c -o decode.o

decode: decode.o
	$(CC) -Wall -g decode.o -o decode

shell.o: stshell.c
	$(CC) -c -Wall -g stshell.c -o shell.o

stshell: shell.o
	$(CC) -Wall -g shell.o -o stshell

clean:
	rm -rf *.o cmp copy encode decode codecA codecB stshell
cleantxt:
	rm -rf *.txt