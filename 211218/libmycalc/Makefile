NAME_PROJECT := mycalc
NAME_LIB := $(addprefix lib, $(addsuffix .so, $(NAME_PROJECT)))

PATH_OBJ := ./objects/
PATH_SRC := ./source/

SRC := $(wildcard $(addsuffix *.c, $(PATH_SRC)))
OBJ := $(patsubst $(PATH_SRC)%.c, $(PATH_OBJ)%.o, $(SRC))

.PHONY: clean

all : libmycalc.a

libmycalc.a : object_dir $(OBJ)
	gcc -shared -o libmycalc.so $(OBJ)

object_dir :
	mkdir -p $(PATH_OBJ)

$(PATH_OBJ)%.o : $(PATH_SRC)%.c
	gcc -c $< -o $@ -fPIC

clean:
	rm -rf $(PATH_OBJ) $(NAME_LIB)