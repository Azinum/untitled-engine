// renderer.c

#if USE_OPENGL
  #include "renderer_opengl.c"
#else
  #include "renderer_null.c"
#endif
