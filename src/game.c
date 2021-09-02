// game.c

#include "game.h"

#include "common.c"
#include "math_util.c"
#include "memory.c"
#include "renderer.c"
#include "platform.c"

static Game game_state;

static i32 game_state_init(Game* game);
static i32 game_run(Game* game);

i32 game_state_init(Game* game) {
  game->running = 1;
  return NO_ERR;
}

i32 game_run(Game* game) {
  while (game->running && platform_handle_events() >= 0) {
    if (key_pressed[KEY_ESCAPE]) {
      game->running = 0;
    }
    platform_swap_buffers();
    renderer_clear(30, 30, 30);
  }
  return NO_ERR;
}

i32 game_start(i32 argc, char** argv) {
  Game* game = &game_state;
  game_state_init(game);
  if (platform_open_window(800, 600, 1, 0, "game") == NO_ERR) {
    renderer_init();
    game_run(game);
    platform_close_window();
    renderer_free();
  }
  assert(memory_total() == 0 && "memory leak!");
  return NO_ERR;
}
