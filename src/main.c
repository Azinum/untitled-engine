// main.c

#include "game.c"

#if _WIN32
  #include "windows_main.c"
#else

i32 main(i32 argc, char** argv) {
#if 0
  zone_memory_init(MB(5));
  pack_dir("tmp/resource.pack", "resource");
  zone_print_all(stdout);
  zone_memory_free();
  return 0;
#else
  return game_start(argc, argv);
#endif
}

#endif
