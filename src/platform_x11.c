// platform_x11.c

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>  // XSizeHints
#include <X11/keysym.h>

#error "X11 platform layer NOT IMPLEMENTED"

Keycode KEY_ESCAPE = XK_Escape;

Keycode KEY_0 = XK_0;
Keycode KEY_1 = XK_1;
Keycode KEY_2 = XK_2;
Keycode KEY_3 = XK_3;
Keycode KEY_4 = XK_4;
Keycode KEY_5 = XK_5;
Keycode KEY_6 = XK_6;
Keycode KEY_7 = XK_7;
Keycode KEY_8 = XK_8;
Keycode KEY_9 = XK_9;

#define MAX_KEY 255

i8 mouse_state = 0;
u8 key_down[MAX_KEY] = {0};
u8 key_pressed[MAX_KEY] = {0};

i32 platform_open_window(i32 width, i32 height, u8 vsync, u8 fullscreen, const char* title) {
  return NO_ERR;
}

i32 platform_handle_events() {
  return 0;
}

void platform_swap_buffers() {

}

i32 platform_close_window() {
  return NO_ERR;
}
