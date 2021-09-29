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

typedef struct Texture {
  u16 width;
  u16 height;
  i32 id;
} Texture;

void renderer_framebuffer_cb(i32 window_width, i32 window_height); // Implemented in renderer.c

void render_texture(const Texture* texture, v3 position, v3 size);

void render_model(i32 model_id, const Texture* texture, v3 position, v3 size);

i32 renderer_upload_mesh(Mesh* mesh);

i32 renderer_upload_texture(Image* source, Texture* texture);

i32 renderer_init();

u32 renderer_num_draw_calls();

void renderer_begin_frame();

void renderer_end_frame(u8 r, u8 g, u8 b);

void renderer_free();

#endif
