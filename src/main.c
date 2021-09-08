// main.c

#include "game.c"

#if _WIN32
  #include "windows_main.c"
#else

i32 main(i32 argc, char** argv) {
  return game_start(argc, argv);
}

#endif
