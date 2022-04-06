// windows_main.c

#include <windows.h>

i32 CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, i32 cmd_show) {
  return game_start(0, NULL);
}
