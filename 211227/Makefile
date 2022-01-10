NAME_PROJECT := mycalc

PATH_OBJ := ./objects/
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, $(PATH_OBJ)%.o, $(SRC))

.PHONY: clean object_dir all

all: calc

calc : object_dir $(OBJ)
	gcc -o $(NAME_PROJECT) $(OBJ) -ldl -g -Wall

object_dir :
	mkdir -p $(PATH_OBJ)

$(PATH_OBJ)%.o : %.c
	gcc -c $< -o $@ -g

clean:
	rm -rf $(NAME_PROJECT) $(PATH_OBJ)