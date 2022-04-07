// config.h
// general game configuration variables

#ifndef _CONFIG_H
#define _CONFIG_H

#define PACK_FILE "build/data.pack"

static i32 g_zone_memory = MB(32);
static i32 g_win_width = 800;
static i32 g_win_height = 600;
static const char* g_game_title = "game";
static i32 g_vsync = 1;
static i32 g_fullscreen = 0;
static i32 g_resizable_window = 0;
static f32 g_dt_max = 0.5f;
static f32 g_camera_fov = 90;
static f32 g_camera_near = 0.1f;
static f32 g_camera_far = 200.0f;
static i32 g_sample_rate = 44100;
static i32 g_frames_per_buffer = 256;
static i32 g_channel_count = 2;

#endif
