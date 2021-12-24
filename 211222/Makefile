NAME_PROJECT := testdl

PATH_OBJ := ./objects/
SRC := $(wildcard *.c)
OBJ := $(patsubst %.c, $(PATH_OBJ)%.o, $(SRC))

.PHONY: clean object_dir all

all: testdl

testdl : object_dir $(OBJ)
	gcc -o $(NAME_PROJECT) $(OBJ) -ldl

object_dir :
	mkdir -p $(PATH_OBJ)

$(PATH_OBJ)%.o : %.c
	gcc -c $< -o $@

clean:
	rm -rf $(NAME_PROJECT) $(PATH_OBJ)