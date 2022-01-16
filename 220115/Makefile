.PHONY: all hw1 hw2 hw3 clean

all: hw1 hw2 hw3

hw1: 220115_hw1.o
	gcc -o hw1 220115_hw1.o -g -std=c89

220115_hw1.o: 220115_hw1.c
	gcc -o 220115_hw1.o -c 220115_hw1.c -g -std=c89

hw2: 220115_hw2.o
	gcc -o hw2 220115_hw2.o -g -std=c89

220115_hw2.o: 220115_hw2.c
	gcc -o 220115_hw2.o -c 220115_hw2.c -g -std=c89

hw3: 220115_hw3.o
	gcc -o hw3 220115_hw3.o -g -std=c89

220115_hw3.o: 220115_hw3.c
	gcc -o 220115_hw3.o -c 220115_hw3.c -g -std=c89

clean:
	rm -rf hw1 hw2 hw3 *.o testFile.txt