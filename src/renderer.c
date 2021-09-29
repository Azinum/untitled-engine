// renderer.c

#if USE_OPENGL
  #include "renderer_opengl.c"
#else
  #include "renderer_null.c"
#endif

void renderer_framebuffer_cb(i32 window_width, i32 window_height) {
  perspective_proj = perspective(g_camera_fov, (f32)window_width / window_height, g_camera_near, g_camera_far);
  orthogonal_proj = orthographic(0, window_width, window_height, 0, -1.0f, 1.0f);
}
