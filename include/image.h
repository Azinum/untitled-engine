// image.h

#ifndef _IMAGE_H
#define _IMAGE_H

typedef struct Image {
  u8* pixels;
  u32 width;
  u32 height;
  u16 bytes_per_pixel;
} Image;

typedef union Color_rgba {
  struct {
    u8 b;
    u8 g;
    u8 r;
    u8 a;
  };
  struct {
    u32 value;
  };
} Color_rgba;

i32 image_init(i32 width, i32 height, u16 bytes_per_pixel, Image* image);

i32 image_load(const char* path, Image* image);

void image_unload(Image* image);

#endif
