// game.h

#ifndef _GAME_H
#define _GAME_H

#include "entity.h"

#define MAX_ENTITY 512

typedef struct Game {
  u8 running;
  Entity entities[MAX_ENTITY];
  u32 entity_count;

  u64 tick;
  f64 dt;
  f64 total_time;
} Game;

i32 game_start(i32 argc, char** argv);

#endif
