.PHONY: all hw1 clean

all: hw1

hw1: 220126_hw1.o
	gcc -o hw1 220126_hw1.o -g -std=c89

220126_hw1.o: 220126_hw1.c
	gcc -o 220126_hw1.o -c 220126_hw1.c -g -std=c89

clean:
	rm -rf hw1 *.o