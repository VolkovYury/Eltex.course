NAME_PROJECT := mycalc

all: calc

calc : 211215_hw1.c calc_func.c
	gcc -o $(NAME_PROJECT) 211215_hw1.c calc_func.c -L./libmycalc/ -l$(NAME_PROJECT)

.PHONY: clean

clean:
	rm -f $(NAME_PROJECT)