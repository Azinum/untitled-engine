// renderer_null.c

void render_texture(const Texture* texture, v3 position, v3 size) {

}

void render_mesh(i32 model_id, const Texture* texture, v3 position, v3 size) {

}

u32 renderer_push_quad(v3 position, v3 size, v2 uv_offset, v2 uv_range) {
  return 0;
}

void renderer_draw() {

}

i32 renderer_upload_mesh(Mesh* mesh) {
  return -1;
}

i32 renderer_upload_texture(Image* source, Texture* texture) {
  return -1;
}

i32 renderer_init() {
  return NO_ERR;
}

u32 renderer_num_draw_calls() {
  return 0;
}

void renderer_begin_frame() {

}

void renderer_end_frame(u8 r, u8 g, u8 b) {

}

void renderer_free() {

}
