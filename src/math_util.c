// math_util.c

inline m4 m4d(float value) {
  m4 result = {0};

  result.e[0][0] = value;
  result.e[1][1] = value;
  result.e[2][2] = value;
  result.e[3][3] = value;

  return result;
}

inline m4 m4_multiply(m4 a, m4 b) {
  m4 result = {0};

  for (i32 col = 0; col < 4; ++col) {
    for (i32 row = 0; row < 4; ++row) {
      float sum = 0;
      for (i32 current = 0; current < 4; ++current) {
        sum += a.e[current][row] * b.e[col][current];
      }
      result.e[col][row] = sum;
    }
  }
  return result;
}

inline m4 translate(v3 a) {
  m4 result = m4d(1.0f);

  result.e[3][0] = a.x;
  result.e[3][1] = a.y;
  result.e[3][2] = a.z;

  return result;
}

inline m4 scale(v3 a) {
  m4 result = {0};

  result.e[0][0] = a.x;
  result.e[1][1] = a.y;
  result.e[2][2] = a.z;
  result.e[3][3] = 1.0f;

  return result;
}

inline m4 orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 z_near, f32 z_far) {
  m4 result = {0};

  result.e[0][0] = 2.0f / (right - left);
  result.e[1][1] = 2.0f / (top - bottom);
  result.e[2][2] = 2.0f / (z_near - z_far);
  result.e[3][3] = 1.0f;

  result.e[3][0] = (left + right) / (left - right);
  result.e[3][1] = (bottom + top) / (bottom - top);
  result.e[3][2] = (z_far + z_near) / (z_near - z_far);

  return result;
}
