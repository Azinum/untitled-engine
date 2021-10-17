// game.h

#ifndef _GAME_H
#define _GAME_H

#include "common.h"
#include "hash.h"
#include "random.h"
#include "arg_parser.h"
#include "config.h"
#include "memory.h"
#include "zone.h"
#include "pack.h"
#include "math_util.h"
#include "image.h"
#include "bmp.h"
#include "ppm.h"
#include "mesh.h"
#include "audio_engine.h"
#include "renderer.h"
#include "camera.h"
#include "platform.h"
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
