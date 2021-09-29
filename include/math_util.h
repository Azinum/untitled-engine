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

#define V3_OP(_a, _b, _op) V3(_a.x _op _b.x, _a.y _op _b.y, _a.z _op _b.z)
#define V3_OP1(_a, _b, _op) V3(_a.x _op _b, _a.y _op _b, _a.z _op _b)
#define V3(_x, _y, _z) ((v3) { .x = _x, .y = _y, .z = _z })
#define V2(_x, _y) ((v2) { .x = _x, .y = _y })

extern m4 m4d(f32 value);

extern f32 v3_dot(v3 a, v3 b);

extern v3 v3_cross(v3 a, v3 b);

extern f32 v3_length_square(v3 a);

extern f32 v3_length(v3 a);

extern f32 v3_length_normalize(v3 a);

extern v3 v3_normalize(v3 a);

extern v3 v3_normalize_fast(v3 a);

extern f32 fast_inv_sqrt(f32 a);

extern f32 lerp(f32 v0, f32 v1, f32 t);

extern f32 radians(f32 angle);

extern f32 square_root(f32 a);

extern m4 m4_multiply(m4 a, m4 b);

extern m4 translate(v3 a);

extern m4 scale(v3 a);

extern m4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far);

extern m4 perspective(f32 fov, f32 aspect, f32 z_near, f32 z_far);

extern m4 look_at(v3 eye, v3 center, v3 up);

#endif
