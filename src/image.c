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

i32 image_load(const char* path, Image* image) {
  char* ext = get_extension(path);
  if (!strcmp(ext, "bmp")) {
    return bmp_load(path, image);
  }
  else {
    fprintf(stderr, "Image format %s not supported for file %s\n", ext, path);
  }
  return NO_ERR;
}

void image_unload(Image* image) {
  if (image->data) {
    zone_free(image->data);
    image->data = NULL;
    memory_zero(image, sizeof(Image));
  }
}
