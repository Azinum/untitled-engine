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

extern m4 m4d(float value);

extern m4 m4_multiply(m4 a, m4 b);

extern m4 translate(v3 a);

extern m4 scale(v3 a);

extern m4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far);

#endif
