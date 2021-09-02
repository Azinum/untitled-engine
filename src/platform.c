// platform.c

#if USE_X11
  #include "platform_x11.c"
#elif USE_NULL
  #include "platform_null.c"
#else
  #include "platform_glfw.c"
#endif
