// game.h

#ifndef _GAME_H
#define _GAME_H

#include "common.h"
#include "math_util.h"
#include "memory.h"
#include "renderer.h"
#include "platform.h"

typedef struct Game {
  u8 running;
} Game;

i32 game_start(i32 argc, char** argv);

#endif
