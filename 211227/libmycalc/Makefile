NAME_PROJECT := mycalc
NAME_LIB := $(addprefix lib, $(addsuffix .so, $(NAME_PROJECT)))

PATH_OBJ := ./objects/
PATH_SRC := ./source/
PATH_PLUGINS := ../plugins/

SRC := $(wildcard $(addsuffix *.c, $(PATH_SRC)))
OBJ := $(patsubst $(PATH_SRC)%.c, $(PATH_OBJ)%.o, $(SRC))
PLUG := $(patsubst $(PATH_SRC)%.c, $(PATH_PLUGINS)%.so, $(SRC))

.PHONY: clean object_dir plugins_dir all

all : object_dir plugins_dir $(OBJ) $(PLUG)

$(PATH_PLUGINS)%.so: $(PATH_OBJ)%.o
	gcc -shared -o $@ $<

$(PATH_OBJ)%.o : $(PATH_SRC)%.c
	gcc -c $< -o $@ -fPIC

object_dir :
	mkdir -p $(PATH_OBJ)

plugins_dir :
	mkdir -p $(PATH_PLUGINS)

clean:
	rm -rf $(PATH_OBJ) $(PATH_PLUGINS)