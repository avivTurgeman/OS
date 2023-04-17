CC = gcc
CFLAGS = -c -Wall -Werror -fPIC
LDFLAGS = -shared -Wall
RM = rm -f


.PHONY: all
all: libcodecA.so

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


.PHONY: clean
clean:
	rm -rf *.o
	rm -rf *.so