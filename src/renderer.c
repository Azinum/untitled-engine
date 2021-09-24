// renderer.c

#if USE_OPENGL
  #include "renderer_opengl.c"
#else
  #include "renderer_null.c"
#endif

#define FOV 60
#define Z_NEAR 0.1f
#define Z_FAR 300.0f

void renderer_framebuffer_cb(i32 window_width, i32 window_height) {
  perspective_proj = perspective(FOV, (float)window_width / window_height, Z_NEAR, Z_FAR);
  orthogonal_proj = orthographic(0, window_width, window_height, 0, -1.0f, 1.0f);
}
