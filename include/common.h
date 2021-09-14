// common.h

#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <assert.h>

typedef float f32;
typedef double f64;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;

#define ARR_SIZE(ARR) ((sizeof(ARR)) / (sizeof(ARR[0])))
#define MAX_PATH_SIZE 512

#define NO_ERR (0)
#define ERR (-1)

#define KB(n) (n * 1024)
#define MB(n) (KB(n * 1024))
#define GB(n) (MB(n * 1024))

typedef struct Buffer {
  char* data;
  u32 size;
} Buffer;

void buffer_free(Buffer* buffer);

// Same as memcpy, destination and source memory areas must not overlap
void* memory_copy(void* restrict dest, const void* restrict source, const u32 size);

void memory_zero(void* restrict dest, const u32 size);

void memory_set(void* restrict dest, const u8 value, const u32 size);

char* get_extension(const char* path);

i32 string_cmp(const char* s0, const char* s1);

i32 string_n_cmp(const char* s0, const char* s1, u32 size);

i32 read_file(const char* path, Buffer* buffer);

#endif
