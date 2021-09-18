// game.c

#include "game.h"

#include "common.c"
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

static i32 game_state_init(Game* game);
static i32 game_run(Game* game);

i32 game_state_init(Game* game) {
  game->running = 1;
  game->entity_count = 0;
  return NO_ERR;
}

i32 game_run(Game* game) {
  Mesh cube_mesh;
  mesh_load("resource/mesh/cube.obj", &cube_mesh);
  i32 cube_id = renderer_upload_mesh(&cube_mesh);

  Image texture;
  image_load("resource/texture/stone_ground.bmp", &texture);
  i32 texture_id = renderer_upload_texture(&texture);
  image_unload(&texture);

  v3 p = V3(0, -2, -5);
  u64 start = 0;
  u64 delta = 0;
  (void)delta;

  while (game->running && platform_handle_events() >= 0) {
    start = read_tsc();
    if (key_pressed[KEY_ESCAPE]) {
      game->running = 0;
    }
    if (key_pressed[KEY_0]) {
      zone_print_all(stdout);
    }
    if (key_pressed[KEY_A]) {
      p.x -= 1;
    }
    if (key_pressed[KEY_D]) {
      p.x += 1;
    }
    if (key_pressed[KEY_W]) {
      p.z -= 1;
    }
    if (key_pressed[KEY_S]) {
      p.z += 1;
    }
    if (key_pressed[KEY_Z]) {
      p.y += 1;
    }
    if (key_pressed[KEY_X]) {
      p.y -= 1;
    }
    renderer_begin_frame();

    render_model(cube_id, texture_id, p, V3(1, 1, 1));

    renderer_end_frame(30, 30, 30);
    delta = read_tsc() - start;
  }
  mesh_unload(&cube_mesh);
  return NO_ERR;
}

i32 game_start(i32 argc, char** argv) {
  zone_memory_init(ZONE_MEMORY);

  Game* game = zone_malloc(sizeof(Game));
  game_state_init(game);

  if (platform_open_window(WIN_WIDTH, WIN_HEIGHT, 1, 0, "game") == NO_ERR) {
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
