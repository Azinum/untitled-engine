// game.c

#include "game.h"

#include "entity.c"

#define MAX_TITLE_SIZE 128
#define SEED 811
#define CAMERA_LERP_SPEED 42.0f
#define CAMERA_ROTATION_LERP_SPEED 42.0f

static i32 game_state_init(Game* game);
static i32 game_load_assets(Game* game);
static void game_unload_assets(Game* game);
static i32 game_run(Game* game);

#define MAP_W 16
#define MAP_H 16

i32 game_state_init(Game* game) {
  random_init(SEED);
  random_push_generator(RANDOM_LC);

  game->running = 1;
  game->entity_count = 0;

  Entity* null = entity_push(game, V3(0, 0, 0));
  null->scale = V3(0, 0, 0);

  game->tick = 0;
  game->dt = 0;
  game->total_time = 0;

  camera_init(V3(3, 0, 3), PERSPECTIVE);

{
  Entity* head = entity_push(game, V3(6, 0, 4));
  head->type = ENTITY_BOBBER;
  head->mesh_id = MESH_SPHERE;
  head->texture_id = TEXTURE_HEAD;
}
{
  Entity* head = entity_push(game, V3(2, 0, 8));
  head->type = ENTITY_BOBBER;
  head->mesh_id = MESH_SPHERE;
  head->texture_id = TEXTURE_HEAD;
}
{
  Entity* horror = entity_push(game, V3(2, 0, 6));
  horror->type = ENTITY_BOBBER;
  horror->mesh_id = MESH_HORROR;
  horror->texture_id = TEXTURE_HORROR;
}
  for (i32 y = 0; y < MAP_H; ++y) {
    for (i32 x = 0; x < MAP_W; ++x) {
      Entity* e = entity_push(game, V3(x, -1, y));
      e->mesh_id = MESH_CUBE;
      e->texture_id = TEXTURE_STONE_GROUND;
      if ((random_number() % 100) < 15) {
        Entity* e = entity_push(game, V3(x, 0, y));
        e->mesh_id = MESH_CUBE;
        e->texture_id = TEXTURE_STONE_GROUND;
      }
    }
  }

  return NO_ERR;
}

i32 game_load_assets(Game* game) {
  Assets* assets = &game->assets;
  for (u32 i = 0; i < MAX_TEXTURE; ++i, ++assets->texture_count) {
    Texture* t = &assets->texture[i];
    Image source;
    image_load_from_pack(texture_path[i], PACK_FILE, &source);
    renderer_upload_texture(&source, t);
    image_unload(&source);
  }

  for (u32 i = 0; i < MAX_MESH; ++i, ++assets->mesh_count) {
    i32* mesh_id = &assets->mesh[i];
    Mesh mesh;
    mesh_load(mesh_path[i], &mesh);
    *mesh_id = renderer_upload_mesh(&mesh);
    mesh_unload(&mesh);
  }
  for (u32 i = 0; i < MAX_AUDIO; ++i, ++assets->audio_count) {
    i32* audio_id = &assets->mesh[i];
    Audio_source audio = {0};
    if (audio_load_from_pack("data/audio/track-01.ogg", PACK_FILE, &audio) == NO_ERR) {
      *audio_id = audio_engine_push_audio_source(&audio);
    }
  }
  return NO_ERR;
}

void game_unload_assets(Game* game) {
  Assets* assets = &game->assets;
  assets->texture_count = 0;
  assets->mesh_count = 0;
  assets->audio_count = 0;
}

i32 game_run(Game* game) {
  game_load_assets(game);
  char title[MAX_TITLE_SIZE] = {0};
  f64 now = 0;
  f64 last = 0;

  // audio_engine_play_audio_once(game->assets.audio[AUDIO_TRACK_01], AUDIO_BUS_MASTER, 0.2f);

  while (game->running && platform_handle_events() >= 0 && !platform_window_should_close()) {
    last = now;
    now = platform_get_time();
    game->dt = now - last;
    game->total_time += game->dt;
    if (game->dt > g_dt_max) {
      game->dt = g_dt_max;
    }
    ++game->tick;
    if (!(game->tick % 32)) {
      snprintf(title, MAX_TITLE_SIZE, "%s | fps: %i | dt: %g | draw calls: %u", g_game_title, (i32)(1.0f / game->dt), game->dt, renderer_num_draw_calls());
      platform_window_set_title(title);
    }

    if (key_pressed[KEY_ESCAPE]) {
      game->running = 0;
    }
    if (key_pressed[KEY_0]) {
      zone_print_all(stdout);
    }
    if (key_pressed[KEY_R]) {
      game_state_init(game);
      continue;
    }
    if (key_pressed[KEY_P]) {
      camera_set_projection_mode(PERSPECTIVE);
    }
    if (key_pressed[KEY_O]) {
      camera_set_projection_mode(ORTHOGONAL);
    }
    if (key_pressed[KEY_A]) {
      camera.rotation_target.yaw += -90;
    }
    if (key_pressed[KEY_D]) {
      camera.rotation_target.yaw += 90;
    }
    if (key_pressed[KEY_W]) {
      camera.target = V3_OP(
        V3(
          camera.forward.x,
          0,
          camera.forward.z
        ),
        camera.target,
        +
      );
    }
    if (key_pressed[KEY_S]) {
      camera.target = V3_OP(
        V3(
          -camera.forward.x,
          0,
          -camera.forward.z
        ),
        camera.target,
        +
      );
    }
    if (key_pressed[KEY_Z]) {
      camera.target.y -= 1;
    }
    if (key_pressed[KEY_X]) {
      camera.target.y += 1;
    }
    camera.pos = V3(
      lerp(camera.pos.x, camera.target.x, CAMERA_LERP_SPEED * game->dt),
      lerp(camera.pos.y, camera.target.y, CAMERA_LERP_SPEED * game->dt),
      lerp(camera.pos.z, camera.target.z, CAMERA_LERP_SPEED * game->dt)
    );

    camera.rotation = V3(
      lerp(camera.rotation.pitch, camera.rotation_target.pitch, CAMERA_ROTATION_LERP_SPEED * game->dt),
      lerp(camera.rotation.yaw, camera.rotation_target.yaw, CAMERA_ROTATION_LERP_SPEED * game->dt),
      lerp(camera.rotation.roll, camera.rotation_target.roll, CAMERA_ROTATION_LERP_SPEED * game->dt)
    );

    camera_update();

    renderer_begin_frame();

    entities_update_and_render(game);

    renderer_end_frame(30, 30, 30);
  }
  game_unload_assets(game);
  return NO_ERR;
}

typedef struct Game_args {
  i32 pack;
  char* pack_file;
  char* pack_path;
} Game_args;

i32 game_start(i32 argc, char** argv) {
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
      zone_memory_init(MB(512));
      pack_dir(game_args.pack_file, game_args.pack_path);
      zone_memory_free();
    }
  }

  zone_memory_init(g_zone_memory);

  Game* game = zone_malloc(sizeof(Game));
  game_state_init(game);
  audio_engine_state_init(g_sample_rate, g_frames_per_buffer, g_channel_count);

  if (platform_open_window(g_win_width, g_win_height, g_vsync, g_fullscreen, g_resizable_window, g_game_title) == NO_ERR) {
    platform_set_framebuffer_callback(renderer_framebuffer_cb);
    renderer_init();
    game_run(game);
    platform_close_window();
    renderer_free();
  }
  audio_engine_terminate();
  zone_free(game);
  zone_memory_free();
  assert(memory_total() == 0 && "memory leak!");
  return NO_ERR;
}
