// renderer.c

#if USE_OPENGL
  #include "renderer_opengl.c"
#else
  #include "renderer_null.c"
#endif

void renderer_framebuffer_cb(i32 window_width, i32 window_height) {
  camera_update_projection();
}
