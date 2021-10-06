// main.c

#include "game.c"

#if _WIN32
  #include "windows_main.c"
#else

typedef struct Game_args {
  i32 pack;
  char* pack_file;
  char* pack_path;
} Game_args;

i32 main(i32 argc, char** argv) {
#if 0
  random_init(time(NULL));
  random_push_generator(RANDOM_LC);

  const u32 iterations = 10000000;
  const u32 range = 10000;
  f64 test_result = random_test(iterations, range);

  fprintf(stdout, "test_result: %g, iterations: %u, range: %u\n", test_result, iterations, range);

  return 0;
#endif
  Game_args game_args = {
    .pack = 0,
    .pack_file = "build/data.pack",
    .pack_path = "data",
  };
  Parse_arg args[] = {
    {'p', "pack", "pack files", ARG_INT, 0, &game_args.pack},
    {'f', "pack-file", "pack file name", ARG_STRING, 1, &game_args.pack_file},
    {'r', "pack-path", "path to directory to pack", ARG_STRING, 1, &game_args.pack_path},
  };

  i32 result = parse_args(args, ARR_SIZE(args), argc, argv);
  if (result == NO_ERR) {
    if (game_args.pack) {
      zone_memory_init(MB(10), MB(2));
      pack_dir(game_args.pack_file, game_args.pack_path);
      zone_memory_free();
    }
    else {
      return game_start(argc, argv);
    }
  }
  return result;
}

#endif
