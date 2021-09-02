// math_util.h

#ifndef _MATH_UTIL_H
#define _MATH_UTIL_H

#ifndef NO_SSE
  #if __SSE__
    #define USE_SSE 1
    #include <xmmintrin.h>
  #endif
#endif

#define PI32 3.14159265359f

typedef union v3 {
  struct {
    f32 x, y, z;
  };
  struct {
    f32 r, g, b;
  };
} v3;

typedef union v2 {
  struct {
    f32 x, y;
  };
  struct {
    f32 u, v;
  };
} v2;

typedef union m4 {
  f32 e[4][4];
#if USE_SSE
  __m128 rows[4];
#endif
} m4;

#define V3(_x, _y, _z) ((v3) { .x = _x, .y = _y, .z = _z })
#define V2(_x, _y) ((v2) { .x = _x, .y = _y })

#endif
