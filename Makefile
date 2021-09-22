# Makefile

ifeq ("${wildcard config.mk}", "")
$(shell cp default.mk config.mk)
endif

include config.mk
include common.mk

all: compile pack run

prepare:
	mkdir -p ${BUILD}
	${MAKE} -C shader run

compile: prepare
	${CC} ${SRC} ${FLAGS} ${LIBS} ${O_FLAG}

pack:
	./${BUILD}/${PROG} -p

run:
	./${BUILD}/${PROG}

clean:
	rm -dr ${BUILD}/
