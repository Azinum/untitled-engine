// game.h

#ifndef _GAME_H
#define _GAME_H

#include "common.h"
#include "memory.h"
#include "zone.h"
#include "math_util.h"
#include "renderer.h"
#include "platform.h"
#include "entity.h"

#define MAX_ENTITY 512

typedef struct Game {
  u8 running;
  Entity entities[MAX_ENTITY];
  u32 entity_count;
} Game;

i32 game_start(i32 argc, char** argv);

#endif
