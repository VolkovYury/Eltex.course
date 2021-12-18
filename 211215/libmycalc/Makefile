NAME_PROJECT := mycalc
NAME_LIB := $(addprefix lib, $(addsuffix .a, $(NAME_PROJECT)))

PATH_OBJ := ./objects/
PATH_SRC := ./source/

SRC := $(wildcard $(addsuffix *.c, $(PATH_SRC)))
OBJ := $(patsubst $(PATH_SRC)%.c, $(PATH_OBJ)%.o, $(SRC))

all : libmycalc.a

libmycalc.a : $(OBJ)
	ar rc $(NAME_LIB) $(OBJ)

$(PATH_OBJ)%.o : $(PATH_SRC)%.c
	gcc -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) $(NAME_LIB)