// main.c

#include "game.c"

i32 main(i32 argc, char** argv) {
  zone_memory_init(MB(50));
  zone_memory_free();
  return game_start(argc, argv);
}
