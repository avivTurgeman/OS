CC = gcc
CFLAGS = -c -Wall -Werror -fPIC
LDFLAGS = -shared -Wall
RM = rm -f


.PHONY: all clean setup

all: setup libcodecA.so libcodecB.so encode decode

setup:
	export LD_LIBRARY_PATH=/home/alon/CLionProjects/OS/Assignments/Assingment\ 2/:$LD_LIBRARY_PATH

codecA.o: codecA.c
	$(CC) ${CFLAGS}  codecA.c -o codecA.o

libcodecA.so: codecA.o
	$(CC) ${LDFLAGS} codecA.o -o libcodecA.so
	export LD_LIBRARY_PATH=/home/alon/CLionProjects/OS/Assignments/Assingment\ 2/:$LD_LIBRARY_PATH

codecB.o: codecB.c
	$(CC) ${CFLAGS}  codecB.c -o codecB.o

libcodecB.so: codecB.o
	$(CC) ${LDFLAGS} codecB.o -o libcodecB.so
	export LD_LIBRARY_PATH=/home/alon/CLionProjects/OS/Assignments/Assingment\ 2/:$LD_LIBRARY_PATH

encode.o: encode.c
	$(CC) -c -Wall encode.c -o encode.o

encode: encode.o
	$(CC) -Wall encode.o -o encode

decode.o: decode.c
	$(CC) -c -Wall decode.c -o decode.o

decode: decode.o
	$(CC) -Wall decode.o -o decode


clean:
	rm -rf *.o *.so *.exe encode decode