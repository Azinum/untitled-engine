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
	-./tools/pack/pack.sh build/data.pack data_pack > src/pack_file.c 2> /dev/null

compile: prepare
	${CC} ${SRC} ${FLAGS} ${LIBS} ${O_FLAG}

pack:
	./${BUILD}/${PROG} -p

run:
	./${BUILD}/${PROG}

clean:
	rm -dr ${BUILD}/
