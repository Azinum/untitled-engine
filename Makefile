# Makefile

ifeq ("${wildcard config.mk}", "")
$(shell cp default.mk config.mk)
endif

include config.mk
include platform.mk

all: compile pack

prepare:
ifneq (${PLATFORM}, WINDOWS)
	mkdir -p ${BUILD}
	${MAKE} -C shader
#	-./tools/pack/pack.sh build/data.pack data_pack > src/pack_file.c 2> /dev/null
endif

compile: prepare
	${CC} ${SRC} ${FLAGS} ${LIBS} ${O_FLAG}

debug: O_FLAG=${O_DEBUG}
debug: compile
	${DEBUG_PROG} ./${BUILD}/${PROG}

pack:
	./${BUILD}/${PROG} -p

run:
	./${BUILD}/${PROG}

clean:
	rm -dr ${BUILD}/
