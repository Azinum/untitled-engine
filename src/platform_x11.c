// platform_x11.c

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>  // XSizeHints
#include <X11/keysym.h>

#error "X11 platform layer NOT IMPLEMENTED"

#define MAX_KEY 255

i8 mouse_state = 0;
u8 key_down[MAX_KEY] = {0};
u8 key_pressed[MAX_KEY] = {0};

i32 platform_open_window(i32 width, i32 height, u8 vsync, u8 fullscreen, u8 resizeable, const char* title) {
  return ERR;
}

i32 platform_initialize_renderer() {
  return ERR;
}

void platform_set_framebuffer_callback(FbCallback fb_callback) {

}

f64 platform_get_time() {
  return 0;
}

i32 platform_handle_events() {
  return 0;
}

void platform_swap_buffers() {

}

i32 platform_window_width() {
  return 0;
}

i32 platform_window_height() {
  return 0;
}

void platform_window_set_title(const char* title) {

}

i32 platform_close_window() {
  return ERR;
}
