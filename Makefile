INCLUDES= -I ./include
FLAGS= -g
all: 
	gcc ${FLAGS} ${INCLUDES} ./src/main.c -lSDL -o  ./bin/main