// game.c

#include "game.h"

#include "common.c"
#include "arg_parser.c"
#include "config.c"
#include "memory.c"
#include "zone.c"
#include "pack.c"
#include "math_util.c"
#include "image.c"
#include "bmp.c"
#include "mesh.c"
#include "renderer.c"
#include "platform.c"
#include "entity.c"

#include <sys/time.h> // gettimeofday

#define MAX_TITLE_SIZE 128

static i32 game_state_init(Game* game);
static i32 game_run(Game* game);

#define MAP_W 8
#define MAP_H 8
u8 map[MAP_H][MAP_W] = {
  {1, 1, 1, 1, 1, 1, 1, 1},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 1, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 0, 0, 0, 1, 0},
  {1, 1, 0, 0, 0, 0, 1, 0},
  {1, 0, 0, 0, 0, 1, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

i32 game_state_init(Game* game) {
  game->running = 1;
  game->entity_count = 0;
  game->dt = 0;
  return NO_ERR;
}

i32 game_run(Game* game) {
  i32 cube_id = -1;
  i32 texture_id = -1;
  i32 sheet_id = -1;

{
  Image texture;
  image_load_from_pack("data/texture/stone_ground.bmp", PACK_FILE, &texture);
  texture_id = renderer_upload_texture(&texture);
  image_unload(&texture);
}

{
  Image texture;
  image_load_from_pack("data/sprite/sheet.bmp", PACK_FILE, &texture);
  sheet_id = renderer_upload_texture(&texture);
  image_unload(&texture);
}
  Mesh mesh;
  mesh_load("data/mesh/cube.obj", &mesh);
  cube_id = renderer_upload_mesh(&mesh);
  mesh_unload(&mesh);

  v3 p = V3(MAP_W / 2, 0, MAP_H);
  v3 target_p = p;

  char title[MAX_TITLE_SIZE] = {0};
  struct timeval time_now = {0};
  struct timeval time_last = {0};
  f64 last = 0;
  u32 tick = 0;

  while (game->running && platform_handle_events() >= 0) {
    time_last = time_now;
    gettimeofday(&time_now, NULL);
    game->dt = ((((time_now.tv_sec - time_last.tv_sec) * 1000000.0f) + time_now.tv_usec) - (time_last.tv_usec)) / 1000000.0f;
    if (game->dt > 0.5) {
      game->dt = 0.5f;
    }
    last += game->dt;
    ++tick;
    if (!(tick % 30)) {
      snprintf(title, MAX_TITLE_SIZE, "%s | fps: %i", GAME_TITLE, (i32)(1.0f / game->dt));
      platform_window_set_title(title);
    }

    if (key_pressed[KEY_ESCAPE]) {
      game->running = 0;
    }
    if (key_pressed[KEY_0]) {
      zone_print_all(stdout);
    }
    if (key_pressed[KEY_A]) {
      target_p.x -= 1;
    }
    if (key_pressed[KEY_D]) {
      target_p.x += 1;
    }
    if (key_pressed[KEY_W]) {
      target_p.z -= 1;
    }
    if (key_pressed[KEY_S]) {
      target_p.z += 1;
    }
    if (key_pressed[KEY_Z]) {
      target_p.y -= 1;
    }
    if (key_pressed[KEY_X]) {
      target_p.y += 1;
    }

    p = V3(
      lerp(p.x, target_p.x, 20.0f * game->dt),
      lerp(p.y, target_p.y, 20.0f * game->dt),
      lerp(p.z, target_p.z, 20.0f * game->dt)
    );

    renderer_begin_frame();

    render_texture(sheet_id, V3(32, 32, 0), V3(160*4, 8*4, 1));

    for (i32 y = 0; y < MAP_H; ++y) {
      for (i32 x = 0; x < MAP_W; ++x) {
        v3 pos = V3(
          x - p.x,
          p.y,
          y - p.z
        );
        u8 value = map[y][x];
        if (value) {
          render_model(cube_id, texture_id, pos, V3(1, 1, 1));
        }
        render_model(cube_id, texture_id, V3(pos.x, pos.y + 1, pos.z), V3(1, 1, 1));
        render_model(cube_id, texture_id, V3(pos.x, pos.y - 1, pos.z), V3(1, 1, 1));
      }
    }

    renderer_end_frame(30, 30, 30);
  }
  return NO_ERR;
}

i32 game_start(i32 argc, char** argv) {
  zone_memory_init(ZONE_MEMORY, ZONE_TEMP_MEMORY);

  Game* game = zone_malloc(sizeof(Game));
  game_state_init(game);

  if (platform_open_window(WIN_WIDTH, WIN_HEIGHT, VSYNC, 0, "game") == NO_ERR) {
    platform_set_framebuffer_callback(renderer_framebuffer_cb);
    renderer_init();
    game_run(game);
    platform_close_window();
    renderer_free();
  }
  zone_free(game);
  zone_memory_free();
  assert(memory_total() == 0 && "memory leak!");
  return NO_ERR;
}
