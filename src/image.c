// image.c

i32 image_init(i32 width, i32 height, u16 bytes_per_pixel, Image* image) {
  image->pixels = zone_malloc(bytes_per_pixel * width * height * sizeof(u8));
  if (!image->pixels) {
    return ERR;
  }
  image->width = width;
  image->height = height;
  image->bytes_per_pixel = bytes_per_pixel;
  return NO_ERR;
}

i32 image_load(const char* path, Image* image) {
  char* ext = get_extension(path);
  if (!strcmp(ext, ".bmp")) {
    return bmp_load(path, image);
  }
  else {
    fprintf(stderr, "Image format %s not supported for file %s\n", ext, path);
  }
  return NO_ERR;
}

void image_unload(Image* image) {
  if (image->pixels) {
    zone_free(image->pixels);
    image->pixels = NULL;
    memory_zero(image, sizeof(Image));
  }
}
