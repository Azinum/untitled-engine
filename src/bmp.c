// bmp.c

const u16 bitmap_file_type = ('B' << 0) | ('M' << 8);

typedef struct BMP_file_header {
  u16 file_type;
  u32 file_size;
  u16 reserved1;
  u16 reserved2;
  u32 offset; // Offset to pixel data from the beginning of the file
} __attribute__((packed)) BMP_file_header;

typedef struct BMP_header {
  u32 size;
  i32 width;
  i32 height;
  u16 planes;
  u16 bits_per_pixel;
  u32 compression;
  u32 bmp_size;
  i32 horizontal_resolution;
  i32 vertical_resolution;
  u32 colors_used;
  u32 color_important;
  u32 red_mask;
  u32 green_mask;
  u32 blue_mask;
} __attribute__((packed)) BMP_header;

static i32 iterate_file(void* data, i32 size, FILE* fp);

i32 iterate_file(void* data, i32 size, FILE* fp) {
  return fread(data, size, 1, fp) == size;
}

// NOTE(lucas): Supports only uncompressed 24-bit color depth
i32 bmp_load(const char* path, Image* image) {
  i32 result = NO_ERR;
  FILE* fp = fopen(path, "r");
  if (!fp) {
    return ERR;
  }
  BMP_file_header file_header = {0};
  BMP_header header = {0};

  iterate_file(&file_header, sizeof(file_header), fp);
  iterate_file(&header, sizeof(header), fp);

  if (file_header.file_type == bitmap_file_type) {
    if (file_header.reserved1 == 0 && file_header.reserved2 == 0) {
      if (header.compression == 0) {
        i32 bytes_per_pixel = header.bits_per_pixel / 8;
        i32 width = header.width;
        i32 height = header.height;
        if (image_init(width, height, 4, image) == NO_ERR) {
          fseek(fp, file_header.offset, SEEK_SET);  // Seek to the location of the pixel data
          Color_rgba* start_pixel = (Color_rgba*)&image->pixels[0];
          for (i32 y = height - 1; y >= 0; --y) { // Reading row by row, from bottom to top
            for (i32 x = 0; x < width; ++x) {
              iterate_file(start_pixel + (width * y + x), bytes_per_pixel, fp);
            }
          }
        }
      }
      else {
        fprintf(stderr, "Only uncompressed 24-bit bitmaps are supported\n");
        result = ERR;
        goto done;
      }
    }
    else {
      fprintf(stderr, "Invalid bitmap\n");
      result = ERR;
      goto done;
    }
  }
  else {
    fprintf(stderr, "Invalid bitmap\n");
    result = ERR;
    goto done;
  }

done:
  fclose(fp);
  return result;
}
