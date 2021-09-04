// common.h

#ifndef _COMMON_H
#define _COMMON_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <string.h> // strcmp

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

// Same as memcpy, destination and source memory areas must not overlap
void* memory_copy(void* restrict dest, const void* restrict source, const u32 size);

void memory_zero(void* restrict dest, const u32 size);

char* get_extension(const char* path);

#endif
