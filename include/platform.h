// platform.h

#ifndef _PLATFORM_H
#define _PLATFORM_H

typedef const i32 Keycode;

extern Keycode KEY_ESCAPE;

extern Keycode KEY_0;
extern Keycode KEY_1;
extern Keycode KEY_2;
extern Keycode KEY_3;
extern Keycode KEY_4;
extern Keycode KEY_5;
extern Keycode KEY_6;
extern Keycode KEY_7;
extern Keycode KEY_8;
extern Keycode KEY_9;

extern Keycode KEY_LAST;

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

i32 platform_handle_events();

void platform_swap_buffers();

i32 platform_window_width();

i32 platform_window_height();

i32 platform_close_window();

#endif
