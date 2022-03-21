NAME_PROJECT := myChat

PATH_OBJ_SERVER := ./objects/server/
PATH_OBJ_CLIENT := ./objects/client/

PATH_SRC_SERVER := ./source/server/
PATH_SRC_CLIENT := ./source/client/

SRC_SERVER := $(wildcard $(addsuffix *.c, $(PATH_SRC_SERVER)))
SRC_CLIENT := $(wildcard $(addsuffix *.c, $(PATH_SRC_CLIENT)))

OBJ_SERVER := $(patsubst $(PATH_SRC_SERVER)%.c, $(PATH_OBJ_SERVER)%.o, $(SRC_SERVER))
OBJ_CLIENT := $(patsubst $(PATH_SRC_CLIENT).c, $(PATH_OBJ_CLIENT)%.o, $(SRC_CLIENT))

.PHONY: clean object_dir_server object_dir_client all server client cleanServer cleanServer

all: server client

server : object_dir_server $(OBJ_SERVER)
	gcc -o myServer $(OBJ_SERVER) -g -Wall -std=c89 -pthread -lrt

client : object_dir_client $(OBJ_CLIENT)
	gcc -o myClient $(OBJ_CLIENT) -g -Wall -std=c89 -pthread -lrt

object_dir_server :
	mkdir -p $(PATH_OBJ_SERVER)

object_dir_client :
	mkdir -p $(PATH_OBJ_CLIENT)

$(PATH_OBJ_SERVER)%.o : $(PATH_SRC_SERVER)%.c
	gcc -c $< -o $@ -g -std=c89 -pthread -lrt

$(PATH_OBJ_CLIENT)%.o : $(PATH_SRC_CLIENT)%.c
	gcc -c $< -o $@ -g -std=c89 -pthread -lrt

clean:
	rm -rf myServer myClient ./objects/

cleanServer:
	rm -rf myServer $(PATH_OBJ_SERVER)

cleanClient:
	rm -rf myClient $(PATH_OBJ_CLIENT)