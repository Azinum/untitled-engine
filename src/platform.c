// platform.c

#if USE_X11
  #include "platform_x11.c"
#elif USE_GLFW
  #include "platform_glfw.c"
#elif USE_TERM
  #include "platform_term.c"
#else
  #include "platform_null.c"
#endif
