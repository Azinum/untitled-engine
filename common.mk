# common.mk

MACHINE=${strip ${shell ${CC} -dumpmachine}}
ifndef PLATFORM
	PLATFORM=UNKNOWN
	ifneq (, ${findstring -linux, ${MACHINE}})
		PLATFORM=LINUX
	endif
	ifneq (, ${findstring -freebsd, ${MACHINE}})
		PLATFORM=BSD
	endif
	ifneq (, ${findstring -mingw32, ${MACHINE}})
		PLATFORM=WINDOWS
	endif
	ifneq (, ${findstring -darwin, ${MACHINE}})
		PLATFORM=DARWIN
	endif
endif

ifeq (${PLATFORM}, LINUX)
	LIBS+=-lGL -lGLU
	ifeq (${USE_X11}, 1)
		LIBS+=-lX11
		FLAGS+=-D USE_X11 -I${X11_INC}
	else
		LIBS+=-lglfw
	endif
endif
