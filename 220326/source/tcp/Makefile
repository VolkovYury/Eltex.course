NAME_PROJECT := myTCP

PATH_OBJ_SERVER := ../../objects/tcp/server/
PATH_OBJ_CLIENT := ../../objects/tcp/client/

PATH_SRC_SERVER := ./server/
PATH_SRC_CLIENT := ./client/

SRC_SERVER := $(wildcard $(addsuffix *.c, $(PATH_SRC_SERVER)))
SRC_CLIENT := $(wildcard $(addsuffix *.c, $(PATH_SRC_CLIENT)))

OBJ_SERVER := $(patsubst $(PATH_SRC_SERVER)%.c, $(PATH_OBJ_SERVER)%.o, $(SRC_SERVER))
OBJ_CLIENT := $(patsubst $(PATH_SRC_CLIENT)%.c, $(PATH_OBJ_CLIENT)%.o, $(SRC_CLIENT))

.PHONY: all object_dir_server server object_dir_client client cleanTCP cleanServer cleanServer clean

all: server client

server : object_dir_server $(OBJ_SERVER)
	gcc -o myServer $(OBJ_SERVER) -g -W -Wall -Werror -std=c89

client : object_dir_client $(OBJ_CLIENT)
	gcc -o myClient $(OBJ_CLIENT) -g -W -Wall -Werror -std=c89

object_dir_server :
	mkdir -p $(PATH_OBJ_SERVER)

object_dir_client :
	mkdir -p $(PATH_OBJ_CLIENT)

$(PATH_OBJ_SERVER)%.o : $(PATH_SRC_SERVER)%.c
	gcc -c $< -o $@ -g -W -Wall -Werror -std=c89

$(PATH_OBJ_CLIENT)%.o : $(PATH_SRC_CLIENT)%.c
	gcc -c $< -o $@ -g -W -Wall -Werror -std=c89

cleanTCP:
	rm -rf myServer myClient ../../objects/tcp/

cleanServer:
	rm -rf myServer $(PATH_OBJ_SERVER)

cleanClient:
	rm -rf myClient $(PATH_OBJ_CLIENT)

clean:
	rm -rf myServer myClient ../../objects/