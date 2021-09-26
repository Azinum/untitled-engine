// platform.h

#ifndef _PLATFORM_H
#define _PLATFORM_H

typedef const i32 Keycode;

#define KEYDEF extern Keycode

KEYDEF KEY_ESCAPE;
KEYDEF KEY_0;
KEYDEF KEY_1;
KEYDEF KEY_2;
KEYDEF KEY_3;
KEYDEF KEY_4;
KEYDEF KEY_5;
KEYDEF KEY_6;
KEYDEF KEY_7;
KEYDEF KEY_8;
KEYDEF KEY_9;
KEYDEF KEY_A;
KEYDEF KEY_B;
KEYDEF KEY_C;
KEYDEF KEY_D;
KEYDEF KEY_E;
KEYDEF KEY_F;
KEYDEF KEY_G;
KEYDEF KEY_H;
KEYDEF KEY_I;
KEYDEF KEY_J;
KEYDEF KEY_K;
KEYDEF KEY_L;
KEYDEF KEY_M;
KEYDEF KEY_N;
KEYDEF KEY_O;
KEYDEF KEY_P;
KEYDEF KEY_Q;
KEYDEF KEY_R;
KEYDEF KEY_S;
KEYDEF KEY_T;
KEYDEF KEY_U;
KEYDEF KEY_V;
KEYDEF KEY_W;
KEYDEF KEY_X;
KEYDEF KEY_Y;
KEYDEF KEY_Z;

KEYDEF KEY_LAST;

extern i8 mouse_state;
extern u8 key_down[];
extern u8 key_pressed[];

typedef void (*FbCallback)(i32 width, i32 height);

#define WINDOW_COMMON \
  i32 width; \
  i32 height; \
  u8 fullscreen; \
  FbCallback fb_callback

i32 platform_open_window(i32 width, i32 height, u8 vsync, u8 fullscreen, const char* title);

i32 platform_initialize_renderer();

void platform_set_framebuffer_callback(FbCallback fb_callback);

f64 platform_get_time();

i32 platform_handle_events();

void platform_swap_buffers();

i32 platform_window_width();

i32 platform_window_height();

void platform_window_set_title(const char* title);

i32 platform_close_window();

#endif
