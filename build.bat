
@echo off

set CC=gcc

set LIBS=-lm

set PROG=game

set SRC=game\main.c

set INC=include

set BUILD=build

set FLAGS=-o %BUILD%\%PROG% -I%INC% -Wall -pedantic

%CC% %SRC% %FLAGS% %LIBS% -O3 -ffast-math

%BUILD%\%PROG%.exe -p

%BUILD%\%PROG%.exe
