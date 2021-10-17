// game.c

#include "game.h"

#include "common.c"
#include "hash.c"
#include "random.c"
#include "arg_parser.c"
#include "config.c"
#include "memory.c"
#include "zone.c"
#include "pack.c"
#include "math_util.c"
#include "image.c"
#include "bmp.c"
#include "ppm.c"
#include "mesh.c"
#include "audio_engine.c"
#include "renderer.c"
#include "camera.c"
#include "platform.c"
#include "entity.c"

#define MAX_TITLE_SIZE 128
#define SEED 811

static i32 game_state_init(Game* game);
static i32 game_run(Game* game);

#define MAP_W 10
#define MAP_H 10
static u8 map[MAP_H][MAP_W] = {0};

i32 game_state_init(Game* game) {
  random_init(SEED);
  random_push_generator(RANDOM_LC);

  game->running = 1;
  game->entity_count = 0;

  game->tick = 0;
  game->dt = 0;
  game->total_time = 0;

  camera_init(V3(3, 0, 3), PERSPECTIVE);

  for (i32 y = 0; y < MAP_H; ++y) {
    for (i32 x = 0; x < MAP_W; ++x) {
      u8* block = &map[y][x];
      if ((random_number() % 100) < 15) {
        *block = 1;
      }
      else {
        *block = 0;
      }
    }
  }
  return NO_ERR;
}

i32 game_run(Game* game) {
  i32 cube_id = -1;
  Texture ground_texture;
  Texture sheet_texture;

{
  Image source;
  image_load_from_pack("data/texture/stone_ground.bmp", PACK_FILE, &source);
  renderer_upload_texture(&source, &ground_texture);
  image_unload(&source);
}
{
  Image source;
  image_load_from_pack("data/sprite/sheet.bmp", PACK_FILE, &source);
  renderer_upload_texture(&source, &sheet_texture);
  image_unload(&source);
}
  Mesh mesh;
  mesh_load("data/mesh/cube.obj", &mesh);
  cube_id = renderer_upload_mesh(&mesh);
  mesh_unload(&mesh);

  char title[MAX_TITLE_SIZE] = {0};
  f64 now = 0;
  f64 last = 0;

  while (game->running && platform_handle_events() >= 0) {
    last = now;
    now = platform_get_time();
    game->dt = now - last;
    game->total_time += game->dt;
    if (game->dt > g_dt_max) {
      game->dt = g_dt_max;
    }
    ++game->tick;
    if (!(game->tick % 16)) {
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
      camera.yaw -= 90;
    }
    if (key_pressed[KEY_D]) {
      camera.yaw += 90;
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
      lerp(camera.pos.x, camera.target.x, 50.0f * game->dt),
      lerp(camera.pos.y, camera.target.y, 50.0f * game->dt),
      lerp(camera.pos.z, camera.target.z, 50.0f * game->dt)
    );

    camera_update();

    renderer_begin_frame();

    for (i32 y = 0; y < MAP_H; ++y) {
      for (i32 x = 0; x < MAP_W; ++x) {
        v3 pos = V3(
          x, 0, y
        );
        u8 value = map[y][x];
        if (value) {
          render_model(cube_id, &ground_texture, pos, V3(1, 1, 1));
        }
        render_model(cube_id, &ground_texture, V3(pos.x, pos.y + 1, pos.z), V3(1, 1, 1));
        render_model(cube_id, &ground_texture, V3(pos.x, pos.y - 1, pos.z), V3(1, 1, 1));
      }
    }
    renderer_push_quad(V3(0, 0, 0), V3(1, 1, 1), V2(0, 0), V2(1, 1));
    renderer_draw();
    renderer_end_frame(30, 30, 30);
  }
  return NO_ERR;
}

i32 game_start(i32 argc, char** argv) {
  zone_memory_init(ZONE_MEMORY, ZONE_TEMP_MEMORY);

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
