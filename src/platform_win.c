// platform_win.c

Keycode KEY_ESCAPE;

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

#define MAX_KEY 255
Keycode KEY_LAST = MAX_KEY;

i8 mouse_state = 0;
u8 key_down[MAX_KEY] = {0};
u8 key_pressed[MAX_KEY] = {0};

i32 platform_open_window(i32 width, i32 height, u8 vsync, u8 fullscreen, const char* title) {
  return NO_ERR;
}

i32 platform_initialize_renderer() {
  return NO_ERR;
}

void platform_set_framebuffer_callback(FbCallback fb_callback) {

}

i32 platform_handle_events() {
  return 0;
}

void platform_swap_buffers() {

}

i32 platform_close_window() {
  return NO_ERR;
}
