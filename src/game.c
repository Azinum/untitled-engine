// game.c

#include "game.h"

#include "common.c"
#include "config.c"
#include "memory.c"
#include "zone.c"
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

  while (game->running && platform_handle_events() >= 0) {
    if (key_pressed[KEY_ESCAPE]) {
      game->running = 0;
    }
    if (key_pressed[KEY_0]) {
      zone_print_all(stdout);
    }
    renderer_begin_frame();

    // render_rect(V3(25, 25, 0), V3(64, 64, 1));

    render_model(cube_id, V3(0, 3, -10), V3(1, 1, 1));

    renderer_end_frame(30, 30, 30);
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
