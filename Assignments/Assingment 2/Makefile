CC = gcc
CFLAGS = -c -Wall -Werror -fPIC
LDFLAGS = -shared -Wall -ldl
RM = rm -f


.PHONY: all clean setup

all: codecA codecB encode decode stshell


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
	$(CC) -Wall encode.o -o encode

decode.o: decode.c
	$(CC) -c -Wall decode.c -o decode.o

decode: decode.o
	$(CC) -Wall decode.o -o decode

shell.o: basic_shell.c
	$(CC) -c -Wall basic_shell.c -o shell.o

stshell: shell.o
	$(CC) -Wall shell.o -o stshell


clean:
	rm -rf *.o encode decode codecA codecB stshell