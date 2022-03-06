NAME_PROJECT := myPipe

PATH_OBJ := ./objects/
PATH_SRC := ./source/

SRC := $(wildcard $(addsuffix *.c, $(PATH_SRC)))
OBJ_S := ./objects/myPipeS.o
OBJ_R := ./objects/myPipeR.o

.PHONY: clean object_dir all pipeSend pipeReceive

all: pipeSend pipeReceive

pipeSend : object_dir $(OBJ_S)
	gcc -o myPipeS $(OBJ_S) -g -Wall -std=c89

pipeReceive : object_dir $(OBJ_R)
	gcc -o myPipeR $(OBJ_R) -g -Wall -std=c89

object_dir :
	mkdir -p $(PATH_OBJ)

$(PATH_OBJ)%.o : $(PATH_SRC)%.c
	gcc -c $< -o $@ -g -std=c89

clean:
	rm -rf myPipeS myPipeR $(PATH_OBJ)
