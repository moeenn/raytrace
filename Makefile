NAME=raytrace
CC=cc
STD=c2x
CFLAGS=-Wall -Wextra -pedantic -Werror -O3
INC=-I./include
LDDFLAGS=-lraylib -lm -lpthread -ldl

SRC=./src/main.c
TARGET=./bin/${NAME}

main:
	${CC} ${SRC} -std=${STD} ${INC} ${CFLAGS} ${LDDFLAGS} -o ${TARGET}

run: main
	@${TARGET}
