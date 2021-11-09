# Makefile

CC=gcc

PROG=shader_compile

FLAGS=-o ${PROG} -Wall -O3

SRC=shader_compile.c

all: compile run

prepare:

compile: prepare
	${CC} ${SRC} ${FLAGS}

run:
	./compile.sh
	cp shader.c ../src
	cp shader.h ../include
