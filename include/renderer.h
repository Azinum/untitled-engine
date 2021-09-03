// renderer.h

#ifndef _RENDERER_H
#define _RENDERER_H

#include "renderer_opengl.h"

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

#endif
