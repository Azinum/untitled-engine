// game.h

#ifndef _GAME_H
#define _GAME_H

#include "entity.h"

#define MAX_ENTITY 1024

enum Texture_id {
  TEXTURE_STONE_GROUND = 0,
  TEXTURE_HEAD,
  TEXTURE_HORROR,

  MAX_TEXTURE,
};

enum Mesh_id {
  MESH_CUBE = 0,
  MESH_SPHERE,
  MESH_HORROR,

  MAX_MESH,
};

enum Audio_id {
  AUDIO_TRACK_01 = 0,

  MAX_AUDIO,
};

const char* texture_path[MAX_TEXTURE] = {
  "data/texture/stone_ground.bmp",
  "data/texture/head.bmp",
  "data/texture/anglerfish.bmp",
};

const char* mesh_path[MAX_MESH] = {
  "data/mesh/cube.obj",
  "data/mesh/sphere.obj",
  "data/mesh/horror.obj",
};

const char* audio_path[MAX_AUDIO] = {
  "data/audio/track-01.ogg",
};

typedef struct Assets {
  Texture texture[MAX_TEXTURE];
  u32 texture_count;

  i32 mesh[MAX_MESH];
  u32 mesh_count;

  i32 audio[MAX_AUDIO];
  u32 audio_count;
} Assets;

typedef struct Game {
  u8 running;
  Entity entities[MAX_ENTITY];
  u32 entity_count;
  u32 entity_id;

  Assets assets;

  u64 tick;
  f64 dt;
  f64 total_time;
} Game;

i32 game_start(i32 argc, char** argv);

#endif
