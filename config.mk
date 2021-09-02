# config.mk

USE_X11?=0
NO_SSE?=0

CC=gcc

LIBS=-lGLEW

PROG=game

BUILD=build

SRC=src/main.c

INC=include

FLAGS=-o ${BUILD}/${PROG} -I${INC} -pedantic -Wall

O_DEBUG=-g -O0

O_RELEASE=-O3 -ffast-math

O_FLAG=${O_RELEASE}

X11_INC=/usr/include/X11
