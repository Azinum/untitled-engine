// platform_term.c

#include <ncurses.h>
#include <sys/ioctl.h>
#include <signal.h>

Keycode KEY_ESCAPE = 27;
Keycode KEY_0 = '0';
Keycode KEY_1 = '1';
Keycode KEY_2 = '2';
Keycode KEY_3 = '3';
Keycode KEY_4 = '4';
Keycode KEY_5 = '5';
Keycode KEY_6 = '6';
Keycode KEY_7 = '7';
Keycode KEY_8 = '8';
Keycode KEY_9 = '9';
Keycode KEY_A = 'a';
Keycode KEY_B = 'b';
Keycode KEY_C = 'c';
Keycode KEY_D = 'd';
Keycode KEY_E = 'e';
Keycode KEY_F = 'f';
Keycode KEY_G = 'g';
Keycode KEY_H = 'h';
Keycode KEY_I = 'i';
Keycode KEY_J = 'j';
Keycode KEY_K = 'k';
Keycode KEY_L = 'l';
Keycode KEY_M = 'm';
Keycode KEY_N = 'n';
Keycode KEY_O = 'o';
Keycode KEY_P = 'p';
Keycode KEY_Q = 'q';
Keycode KEY_R = 'r';
Keycode KEY_S = 's';
Keycode KEY_T = 't';
Keycode KEY_U = 'u';
Keycode KEY_V = 'v';
Keycode KEY_W = 'w';
Keycode KEY_X = 'x';
Keycode KEY_Y = 'y';
Keycode KEY_Z = 'z';

Keycode KEY_LAST = 255;

#define MAX_KEY 255
#define NO_DELAY 1

i8 mouse_state = 0;
u8 key_down[MAX_KEY] = {0};
u8 key_pressed[MAX_KEY] = {0};

typedef struct Window {
  WINDOW_COMMON;

  WINDOW* window;
} Window;

Window window;

static i32 curses_init();
static void sigwinch_handle(i32 sig);

i32 curses_init() {
  if (window.window) {
    endwin();
  }
  window.window = initscr();
  start_color();
  cbreak();
  noecho();
  nodelay(window.window, NO_DELAY);
  clear();
  return NO_ERR;
}

void sigwinch_handle(i32 sig) {
  platform_window_size(&window.width, &window.height);
  if (window.fb_callback) {
    window.fb_callback(window.width, window.height);
  }
  curses_init();
}

i32 platform_open_window(i32 width, i32 height, u8 vsync, u8 fullscreen, u8 resizeable, const char* title) {
  signal(SIGWINCH, sigwinch_handle);
  window.fb_callback = NULL;
  curses_init();
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

  i32 event = getc(stdin);
  for (i32 key_index = 0; key_index < MAX_KEY; ++key_index) {
    if (event == key_index) {
      key_pressed[key_index] = !key_down[key_index];
      key_down[key_index] = 1;
    }
    else {
      key_down[key_index] = 0;
      key_pressed[key_index] = 0;
    }
  }
  return -(event == 4); // ^D
}

void platform_swap_buffers() {
  for (i32 y = 0; y < window.height; ++y) {
    for (i32 x = 0; x < window.height; ++x) {
      mvaddch(y, x, ' ');
    }
  }
  wrefresh(window.window);
}

i32 platform_window_should_close() {
  return 0;
}

i32 platform_window_size(i32* width, i32* height) {
  struct winsize size;
  if (ioctl(0, TIOCGWINSZ, &size) < 0) {
    fprintf(stderr, "Failed to get window size\n");
    return ERR;
  }
  *width = size.ws_col;
  *height = size.ws_row;
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
  endwin();
  return NO_ERR;
}
