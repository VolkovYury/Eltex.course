NAME_PROJECT := mycalc

PATH_OBJ := ./objects/
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, $(PATH_OBJ)%.o, $(SRC))

.PHONY: clean

all: calc

calc : object_dir $(OBJ)
	gcc -o $(NAME_PROJECT) $(OBJ) -L./libmycalc/ -l$(NAME_PROJECT)

object_dir :
	mkdir -p $(PATH_OBJ)

$(PATH_OBJ)%.o : %.c
	gcc -c $< -o $@

clean:
	rm -rf $(NAME_PROJECT) $(PATH_OBJ)