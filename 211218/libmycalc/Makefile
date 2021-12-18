NAME_PROJECT := mycalc
NAME_LIB := $(addprefix lib, $(addsuffix .a, $(NAME_PROJECT)))

PATH_OBJ := ./objects/
PATH_SRC := ./source/

SRC := $(wildcard $(addsuffix *.c, $(PATH_SRC)))
OBJ := $(patsubst $(PATH_SRC)%.c, $(PATH_OBJ)%.o, $(SRC))

all : libmycalc.a

libmycalc.a : $(OBJ)
	gcc -shared -o libmycalc.so $(OBJ)

$(PATH_OBJ)%.o : $(PATH_SRC)%.c
	gcc -c $< -o $@ -fPIC

.PHONY: clean

clean:
	rm -f $(OBJ) $(NAME_LIB)