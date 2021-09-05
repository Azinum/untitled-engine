// main.c

#include "game.c"

i32 main(i32 argc, char** argv) {
  zone_memory_init(MB(2));

  char* s = zone_malloc(32);
  strcpy(s, "Hello World");
  zone_malloc(1);
  zone_malloc(1);
  zone_malloc(1);

  zone_print_all(stdout);
  zone_memory_free();
  // return game_start(argc, argv);
  return 0;
}
