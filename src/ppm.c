// ppm.c

i32 ppm_write(const char* path, Image* image) {
  const char* magic = "P3"; // Raw format
  const u32 max_value = 255;
  FILE* fp = fopen(path, "wb");
  if (!fp) {
    return ERR;
  }
  fprintf(fp, "%s\n", magic);
  fprintf(fp, "# %s\n", path);
  fprintf(fp, "%u %u\n", image->width, image->height);
  fprintf(fp, "%u\n", max_value);

  for (u32 y = 0; y < image->height; ++y) {
    for (u32 x = 0; x < image->width; ++x) {
      u8* data = (u8*)image_fetch_pixel(x, y, image);
      u8 r = *(data + 0);
      u8 g = *(data + 1);
      u8 b = *(data + 2);
      fprintf(fp, " %u %u %u", r, g, b);
      if (!(x % 4)) {
        fprintf(fp, "\n");
      }
    }
    fprintf(fp, "\n");
  }

  fclose(fp);
  return NO_ERR;
}
