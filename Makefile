# Makefile

include config.mk
include common.mk

all: compile run

prepare:
	mkdir -p ${BUILD}
	cp -r ${RES} ${BUILD}

compile: prepare
	${CC} ${SRC} ${FLAGS} ${LIBS} ${O_FLAG}
	strip ${BUILD}/${PROG}

run:
	./${BUILD}/${PROG}

clean:
	rm -dr ${BUILD}/
