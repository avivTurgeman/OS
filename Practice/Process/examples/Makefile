.PHONY: all
all: task1 friend task2

task1:  main2_1.c
	gcc -o forkExample main2_1.c

friend: friendsChild.c
	gcc -o friend friendsChild.c

task2:  main2_2.c
	gcc -o cloneExample main2_2.c

.PHONY: clean
clean:
	-rm forkExample friend cloneExample 2>/dev/null
