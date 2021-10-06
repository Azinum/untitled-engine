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
  random_t value = time(NULL);
  random_t seed = sdbm_hash((void*)&value, sizeof(random_t));
  random_init(seed);
  random_push_generator(RANDOM_LC);

  const u32 iterations = 10000000;
  const u32 range = 10000;
  f64 quality = random_chi_square_test(iterations, range);

  fprintf(stdout, "quality: %g, iterations: %u, range: %u\n", quality, iterations, range);

  return 0;
#endif
#if 0
  random_init(time(NULL));
  random_push_generator(RANDOM_LC);
  // random_push_generator(RANDOM_XORSHIFT);

  FILE* fp = fopen("tmp/random.ppm", "w");

  const u32 width = 512;
  const u32 height = 512;
  const u32 max_value = 255;

  // Raw ppm format
  fprintf(fp, "P3\n");
  fprintf(fp, "# random.ppm\n");
  fprintf(fp, "%u %u\n", width, height);
  fprintf(fp, "%u\n", max_value);

  u32 count = 0;
  for (u32 y = 0; y < height; ++y) {
    for (u32 x = 0; x < width; ++x) {
      u32 v = random_number() % max_value;
      ++count;
      fprintf(fp, " %u %u %u  ", v, v, v);
      if (!(count % 4)) {
        fprintf(fp, "\n");
      }
    }
  }

  fclose(fp);

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
