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
	ifeq (${USE_X11}, 1)
		LIBS+=-lX11
		FLAGS+=-D USE_X11 -I${X11_INC}
	else ifeq (${USE_GLFW}, 1)
		LIBS+=-lglfw
		FLAGS+=-D USE_GLFW
	endif
	ifeq (${USE_OPENGL}, 1)
		LIBS+=-lGLEW -lGL -lGLU
		FLAGS+=-D USE_OPENGL
	endif
endif

ifeq (${PLATFORM}, WINDOWS)

endif

ifeq (${PLATFORM}, DARWIN)
	FLAGS+=-Wno-missing-braces
	ifeq (${USE_GLFW}, 1)
		LIBS+=-lglfw
		FLAGS+=-D USE_GLFW
	endif
	ifeq (${USE_OPENGL}, 1)
		LIBS+=-lGLEW -framework OpenGL
		FLAGS+=-D USE_OPENGL
	endif
endif
