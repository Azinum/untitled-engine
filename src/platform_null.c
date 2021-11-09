// platform_null.c
// Template platform layer

Keycode KEY_ESCAPE = 0;
Keycode KEY_0 = 0;
Keycode KEY_1 = 0;
Keycode KEY_2 = 0;
Keycode KEY_3 = 0;
Keycode KEY_4 = 0;
Keycode KEY_5 = 0;
Keycode KEY_6 = 0;
Keycode KEY_7 = 0;
Keycode KEY_8 = 0;
Keycode KEY_9 = 0;
Keycode KEY_A = 0;
Keycode KEY_B = 0;
Keycode KEY_C = 0;
Keycode KEY_D = 0;
Keycode KEY_E = 0;
Keycode KEY_F = 0;
Keycode KEY_G = 0;
Keycode KEY_H = 0;
Keycode KEY_I = 0;
Keycode KEY_J = 0;
Keycode KEY_K = 0;
Keycode KEY_L = 0;
Keycode KEY_M = 0;
Keycode KEY_N = 0;
Keycode KEY_O = 0;
Keycode KEY_P = 0;
Keycode KEY_Q = 0;
Keycode KEY_R = 0;
Keycode KEY_S = 0;
Keycode KEY_T = 0;
Keycode KEY_U = 0;
Keycode KEY_V = 0;
Keycode KEY_W = 0;
Keycode KEY_X = 0;
Keycode KEY_Y = 0;
Keycode KEY_Z = 0;

Keycode KEY_LAST = 255;

#define MAX_KEY 255

i8 mouse_state = 0;
u8 key_down[MAX_KEY] = {0};
u8 key_pressed[MAX_KEY] = {0};

typedef struct Window {
  WINDOW_COMMON;

} Window;

Window window;

i32 platform_open_window(i32 width, i32 height, u8 vsync, u8 fullscreen, u8 resizeable, const char* title) {
  window.fb_callback = NULL;
  return NO_ERR;
}

i32 platform_initialize_renderer() {
  return NO_ERR;
}

void platform_set_framebuffer_callback(FbCallback fb_callback) {
  window.fb_callback = fb_callback;
}

f64 platform_get_time() {
  return 0;
}

i32 platform_handle_events() {
  memory_zero(&key_pressed[0], sizeof(key_pressed));
  memory_zero(&key_down[0], sizeof(key_down));
  return 0;
}

void platform_swap_buffers() {
}

i32 platform_window_should_close() {
  return 0;
}

i32 platform_window_size(i32* width, i32* height) {
  *width = window.width;
  *height = window.height;
  return NO_ERR;
}

i32 platform_window_width() {
  return window.width;
}

i32 platform_window_height() {
  return window.height;
}

void platform_window_set_title(const char* title) {

}

i32 platform_close_window() {
  return NO_ERR;
}
