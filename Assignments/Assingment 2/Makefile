CC = gcc
CFLAGS = -c -Wall -Werror -fPIC
LDFLAGS = -shared -Wall
RM = rm -f


.PHONY: all
all: libcodecA.so libcodecB.so encode decode

codecA.o: codecA.c
	$(CC) ${CFLAGS}  codecA.c

libcodecA.so: codecA.o
	$(CC) ${LDFLAGS} codecA.o -o libcodecA.so
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

codecB.o: codecB.c
	$(CC) ${CFLAGS}  codecB.c

libcodecB.so: codecB.o
	$(CC) ${LDFLAGS} codecB.o -o libcodecB.so
	export LD_LIBRARY_PATH=.:$LD_LIBRARY_PATH

encode.o: encode.c
	gcc -c encode.c -o encode.o

encode: encode.o
	gcc encode.o -o encode

decode.o: decode.c
	gcc -c decode.c -o decode.o

decode: decode.o
	gcc decode.o -o decode




.PHONY: clean
clean:
	rm -rf *.o *.so *.exe encode decode