// renderer.h

#ifndef _RENDERER_H
#define _RENDERER_H

#if USE_OPENGL
  #include "renderer_opengl.h"
#else
  #include "renderer_null.h"
#endif

m4 model = {0};
m4 view = {0};
m4 perspective_proj = {0};
m4 orthogonal_proj = {0};

f32 quad_vertices[] = {
  // Vertex,  uv coord
  0.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 0.0f, 1.0f, 0.0f,
  0.0f, 0.0f, 0.0f, 0.0f,

  0.0f, 1.0f, 0.0f, 1.0f,
  1.0f, 1.0f, 1.0f, 1.0f,
  1.0f, 0.0f, 1.0f, 0.0f,
};

i32 renderer_init();

void renderer_clear(u8 r, u8 g, u8 b);

void renderer_free();

void render_rect(v3 position, v3 size);

#endif
