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

// NOTE(lucas): Supports only uncompressed 24-bit color depth
i32 bmp_load(Image* image, Buffer* buffer) {
  i32 result = NO_ERR;

  BMP_file_header file_header = {0};
  BMP_header header = {0};

  u32 iter = 0;

  buffer_iterate(&file_header, buffer, sizeof(BMP_file_header), &iter);
  buffer_iterate(&header, buffer, sizeof(BMP_header), &iter);

  if (file_header.file_type == bitmap_file_type) {
    if (file_header.reserved1 == 0 && file_header.reserved2 == 0) {
      if (header.compression == 0) {
        u32 bytes_per_pixel = header.bits_per_pixel / 8;
        u32 width = header.width;
        u32 height = header.height;
        if (image_init(width, height, 4, image) == NO_ERR) {
          iter = file_header.offset;  // 'Seek' to the location of the pixel data
          Color_bgra* start_pixel = (Color_bgra*)&image->data[0];
          for (i32 y = height - 1; y >= 0; --y) { // Reading row by row, from bottom to top
            for (i32 x = 0; x < width; ++x) {
              u32 location = width * y + x;
              buffer_iterate(start_pixel + location, buffer, bytes_per_pixel, &iter);
              Color_bgra* pixel = start_pixel + location;
              *pixel = bgr_to_rgb_internal(pixel);
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
  return result;
}
