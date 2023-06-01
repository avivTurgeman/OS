
CFLAGS = -Wall -Werror
all: st_pipeline


Part_A.o: Part_A.c Part_A.h
	gcc -c $(CFLAGS) Part_A.c -o Part_A.o -lm

queue.o: queue.c queue.h
	gcc -c $(CFLAGS) queue.c -o queue.o

Part_C.o: Part_C.c Part_C.h queue.o
	gcc -c $(CFLAGS) Part_C.c -o Part_C.o

st_pipeline.o: st_pipeline.c Part_C.o queue.o
	gcc -c $(CFLAGS) st_pipeline.c -o st_pipeline.o

st_pipeline: st_pipeline.o Part_A.o
	gcc st_pipeline.o Part_A.o Part_C.o queue.o -o st_pipeline -lm


clean:
	rm -f *.o st_pipeline
.PHONY: all clean