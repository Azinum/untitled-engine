// config.h
// general game configuration variables

#ifndef _CONFIG_H
#define _CONFIG_H

#define ZONE_MEMORY MB(32)
#define ZONE_TEMP_MEMORY (u32)(0.1 * ZONE_MEMORY)

#define PACK_FILE "build/data.pack"

static i32 g_win_width = 800;
static i32 g_win_height = 600;
static const char* g_game_title = "game";
static i32 g_vsync = 0;
static i32 g_fullscreen = 0;
static f32 g_dt_max = 0.5f;
static f32 g_camera_fov = 90;
static f32 g_camera_near = 0.1f;
static f32 g_camera_far = 200.0f;

#endif
