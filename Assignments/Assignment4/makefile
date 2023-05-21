all: react_server

export LD_LIBRARY_PATH=.

react_server.o: react_server.c st_reactor.h hash.h
	gcc -c react_server.c -o react_server.o

react_server: react_server.o st_reactor.so
	gcc react_server.o -L. st_reactor.so -o react_server

st_reactor.o: st_reactor.c hash.c st_reactor.h hash.h
	gcc -c -fpic st_reactor.c -o st_reactor.o

st_reactor.so: st_reactor.o hash.o
	gcc -shared st_reactor.o hash.o -o st_reactor.so -lpthread

hash.o: hash.c hash.h
	gcc -c hash.c -o hash.o

client:
	gcc client_tst.c -o client

.PHONY: all clean

clean:
	rm -f *.o *.so react_server