# config.mk

USE_X11?=0
USE_GLFW?=1
USE_OPENGL?=1
NO_SSE?=0

USE_PORTAUDIO?=1
USE_SDL?=0

CC=gcc

LIBS=-lm

PROG=game

BUILD=build

SRC=src/main.c

INC=include

RES=data

FLAGS=-o ${BUILD}/${PROG} -I${INC} -Wall -pedantic

O_DEBUG=-g -O0

O_RELEASE=-O3 -ffast-math

O_FLAG=${O_RELEASE}

X11_INC=/usr/include/X11
