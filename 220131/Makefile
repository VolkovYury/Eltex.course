NAME_PROJECT := mybash

PATH_OBJ := ./objects/
PATH_SRC := ./source/

SRC := $(wildcard $(addsuffix *.c, $(PATH_SRC)))
OBJ := $(patsubst $(PATH_SRC)%.c, $(PATH_OBJ)%.o, $(SRC))

.PHONY: clean object_dir all

all: bash

bash : object_dir $(OBJ)
	gcc -o $(NAME_PROJECT) $(OBJ) -g -Wall -std=c89

object_dir :
	mkdir -p $(PATH_OBJ)

$(PATH_OBJ)%.o : $(PATH_SRC)%.c
	gcc -c $< -o $@ -g -std=c89

clean:
	rm -rf $(NAME_PROJECT) $(PATH_OBJ)