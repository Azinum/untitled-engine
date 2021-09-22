// image.c

Color_rgba bgr_to_rgb(Color_bgra* color) {
  return (Color_rgba) {
    .r = color->r,
    .g = color->g,
    .b = color->b,
    .a = 255,
  };
}

Color_bgra bgr_to_rgb_internal(Color_bgra* color) {
  return (Color_bgra) {
    .r = color->b,
    .g = color->g,
    .b = color->r,
    .a = 255,
  };
}

i32 image_init(i32 width, i32 height, u16 bytes_per_pixel, Image* image) {
  image->data = zone_malloc(bytes_per_pixel * width * height);
  if (!image->data) {
    return ERR;
  }
  image->width = width;
  image->height = height;
  image->bytes_per_pixel = bytes_per_pixel;
  return NO_ERR;
}

i32 image_load_from_buffer(const char* ext, Image* image, Buffer* buffer) {
  i32 result = ERR;

  if (!string_cmp(ext, "bmp")) {
    result = bmp_load(image, buffer);
  }
  else {
    fprintf(stderr, "Image format %s not supported for file\n", ext);
  }
  return result;
}

i32 image_load(const char* path, Image* image) {
  i32 result = ERR;
  Buffer buffer = {0};
  if ((result = read_file(path, &buffer)) == NO_ERR) {
    result = image_load_from_buffer(get_extension(path), image, &buffer);
    buffer_free(&buffer);
  }
  return result;
}

i32 image_load_from_pack(const char* path, const char* pack_file, Image* image) {
  i32 result = ERR;
  Buffer buffer = {0};
  if ((result = read_pack_file(path, pack_file, &buffer)) == NO_ERR) {
    result = image_load_from_buffer(get_extension(path), image, &buffer);
    buffer_free(&buffer);
  }
  return result;
}

void image_unload(Image* image) {
  if (image->data) {
    zone_free(image->data);
    image->data = NULL;
    memory_zero(image, sizeof(Image));
  }
}
