// game.c

#include "game.h"

#include "entity.c"

#define MAX_TITLE_SIZE 128
#define SEED 811
#define CAMERA_LERP_SPEED 42.0f
#define CAMERA_ROTATION_LERP_SPEED 42.0f

static i32 game_state_init(Game* game);
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

  return NO_ERR;
}

i32 game_run(Game* game) {
  i32 cube_id = -1;
  i32 sphere_id = -1;
  Texture ground_texture;
  Texture head_texture;

{
  Image source;
  image_load_from_pack("data/texture/stone_ground.bmp", PACK_FILE, &source);
  renderer_upload_texture(&source, &ground_texture);
  image_unload(&source);
}
{
  Image source;
  image_load_from_pack("data/texture/head.bmp", PACK_FILE, &source);
  renderer_upload_texture(&source, &head_texture);
  image_unload(&source);
}
{
  Mesh mesh;
  mesh_load("data/mesh/cube.obj", &mesh);
  cube_id = renderer_upload_mesh(&mesh);
  mesh_unload(&mesh);
}
{
  Mesh mesh;
  mesh_load("data/mesh/sphere.obj", &mesh);
  sphere_id = renderer_upload_mesh(&mesh);
  mesh_unload(&mesh);
}
{
  Entity* head = entity_push(game, V3(6, 0, 4));
  head->type = ENTITY_BOBBER;
  head->model_id = sphere_id;
  head->texture = head_texture;
}
{
  Entity* head = entity_push(game, V3(2, 0, 8));
  head->type = ENTITY_BOBBER;
  head->model_id = sphere_id;
  head->texture = head_texture;
}
  for (i32 y = 0; y < MAP_H; ++y) {
    for (i32 x = 0; x < MAP_W; ++x) {
      Entity* e = entity_push(game, V3(x, -1, y));
      e->model_id = cube_id;
      e->texture = ground_texture;
      if ((random_number() % 100) < 15) {
        Entity* e = entity_push(game, V3(x, 0, y));
        e->model_id = cube_id;
        e->texture = ground_texture;
      }
    }
  }
  char title[MAX_TITLE_SIZE] = {0};
  f64 now = 0;
  f64 last = 0;

  Audio_source audio = {0};
  Audio_source audio2 = {0};
  i32 audio_id = -1;
  i32 audio2_id = -1;
  if (audio_load_from_pack("data/audio/0x0.ogg", PACK_FILE, &audio) == NO_ERR) {
    audio_id = audio_engine_push_audio_source(&audio);
  }
  if (audio_load_from_pack("data/audio/0x1.ogg", PACK_FILE, &audio2) == NO_ERR) {
    audio2_id = audio_engine_push_audio_source(&audio2);
  }
  audio_engine_play_audio_once(audio2_id, AUDIO_BUS_MASTER, 0.5f);
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
  audio_unload(&audio);
  audio_unload(&audio2);
  return NO_ERR;
}

i32 game_start(i32 argc, char** argv) {
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
