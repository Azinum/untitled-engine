// windows_main.c

#include <windows.h>

// TODO(lucas): free allocated memory
i32 CALLBACK WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, i32 cmd_show) {
  i32 result = 0;
  i32 argc = 0;
  LPWSTR* arg_list = CommandLineToArgvW(GetCommandLineW(), &argc);
  char** argv = malloc(sizeof(char*) * argc);

  for (i32 i = 0; i < argc; ++i) {
    i32 length = wcslen(arg_list[i]);
    argv[i] = calloc(length + 1, 1);
    for (i32 j = 0; j < length; ++j) {
      argv[i][j] = arg_list[i][j];
    }
  }
  result = game_start(argc, argv);
  LocalFree(arg_list);
  return result;
}
