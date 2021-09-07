# Makefile

CC=gcc

PROG=shader_compile

FLAGS=-o ${PROG} -Wall -O3

SRC=shader_compile.c

all: compile

prepare:

compile: prepare
	${CC} ${SRC} ${FLAGS}
	strip ${PROG}

run:
	./compile.sh
	cp shader.c ../src
	cp shader.h ../include
