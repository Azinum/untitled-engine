// renderer_null.c

void render_texture(i32 texture_id, v3 position, v3 size) {

}

void render_model(i32 model_id, const Texture* texture, v3 position, v3 size) {

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

void renderer_begin_frame() {

}

void renderer_end_frame(u8 r, u8 g, u8 b) {

}

void renderer_free() {

}
