// common.h

#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>

typedef float f32;
typedef double f64;
typedef int64_t i64;
typedef uint64_t u64;
typedef int32_t i32;
typedef uint32_t u32;
typedef int16_t i16;
typedef uint16_t u16;
typedef int8_t i8;
typedef uint8_t u8;

enum Error_code {
  ERR = -1,
  NO_ERR = 0,

  MAX_ERR_CODE,
};

#define ARR_SIZE(ARR) ((sizeof(ARR)) / (sizeof(ARR[0])))
#define MAX_PATH_SIZE 512

#define safe_scanf(ScanStatus, Iterator, Format, ...) { \
  i32 num_bytes_read = 0; \
  ScanStatus = sscanf(Iterator, Format "%n", __VA_ARGS__, &num_bytes_read); \
  Iterator += num_bytes_read; \
}

#define MAX_LINE_SIZE 256

#define KB(n) (n * 1024)
#define MB(n) (KB(n * 1024))
#define GB(n) (MB(n * 1024))

typedef struct Buffer {
  char* data;
  u32 size;
} Buffer;

#ifdef _MSC_VER
  #include <intrin.h>
#else
  #include <x86intrin.h>
#endif

#define USE_TIMER 1

#if USE_TIMER
  #define TIMER_START(...) \
    clock_t _time_end = 0; \
    clock_t _time_start = clock(); \
    __VA_ARGS__

  #define TIMER_END(...) { \
    _time_end = clock(); \
    f64 _delta_time = ((f64) (_time_end - _time_start)) / CLOCKS_PER_SEC; \
    __VA_ARGS__ \
  }
#else

  #define TIMER_START(...)

  #define TIMER_END(...)

#endif // USE_TIMER

void buffer_free(Buffer* buffer);

u32 buffer_iterate(void* restrict dest, Buffer* source, u32 size, u32* iter);

// Same as memcpy, destination and source memory areas must not overlap
void* memory_copy(void* restrict dest, const void* restrict source, const u32 size);

void memory_zero(void* restrict dest, const u32 size);

void memory_set(void* restrict dest, const u8 value, const u32 size);

char* get_extension(const char* path);

i32 string_cmp(const char* s0, const char* s1);

i32 string_n_cmp(const char* s0, const char* s1, u32 size);

i32 string_len(const char* s);

i32 string_copy(char* restrict dest, const char* restrict source);

u32 string_copy_filename_in_path(char* restrict dest, const char* restrict path, u8* end);

u32 string_filename_in_path(const char* path, u8* end);

i32 read_file(const char* path, Buffer* buffer);

extern u64 read_tsc();

#endif
