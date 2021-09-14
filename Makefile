# Makefile

ifeq ("${wildcard config.mk}", "")
$(shell cp default.mk config.mk)
endif

include config.mk
include common.mk

all: compile run

prepare:
	mkdir -p ${BUILD}
	cp -r ${RES} ${BUILD}

compile: prepare
	${CC} ${SRC} ${FLAGS} ${LIBS} ${O_FLAG}

run:
	./${BUILD}/${PROG}

clean:
	rm -dr ${BUILD}/
