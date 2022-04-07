# platform.mk

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

ifeq (${USE_PORTAUDIO}, 1)
	LIBS+=-lportaudio
	FLAGS+=-D USE_PORTAUDIO
else ifeq (${USE_SDL}, 1)
	LIBS+=-lSDL2
	FLAGS+=-D USE_SDL
endif

ifeq (${USE_GTK}, 1)
	ifeq ($(shell pkg-config --atleast-version=3.0 gtk+-3.0 && echo 1), 1)
		LIBS+=`pkg-config --libs gtk+-3.0`
		FLAGS+=`pkg-config --cflags gtk+-3.0` -D USE_GTK
	else
		USE_GTK=0
	endif
endif

ifeq (${PLATFORM}, LINUX)
	ifeq (${USE_X11}, 1)
		LIBS+=-lX11
		FLAGS+=-D USE_X11 -I${X11_INC}
	else ifeq (${USE_GLFW}, 1)
		LIBS+=-lglfw
		FLAGS+=-D USE_GLFW
	else ifeq (${USE_TERM}, 1)
		LIBS+=-lncurses
		FLAGS+=-D USE_TERM
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
	else ifeq (${USE_TERM}, 1)
		LIBS+=-lncurses
		FLAGS+=-D USE_TERM
	endif
	ifeq (${USE_OPENGL}, 1)
		LIBS+=-lGLEW -framework OpenGL
		FLAGS+=-D USE_OPENGL
	endif
endif
