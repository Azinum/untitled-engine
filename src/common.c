// common.c

void* memory_copy(void* restrict dest, const void* restrict source, const u32 size) {
  u8* d = (u8*)dest;
  u8* s = (u8*)source;
  for (u32 index = 0; index < size; ++index, *d++ = *s++);
  return d;
}

void memory_zero(void* restrict dest, const u32 size) {
  memory_set(dest, 0, size);
}

void memory_set(void* restrict dest, const u8 value, const u32 size) {
  u8* d = (u8*)dest;
  for (u32 index = 0; index < size; ++index, *d++ = value);
}

char* get_extension(const char* path) {
  char* iter = (char*)path;
  do {
    char ch = *iter++;
    if (ch == '.' || ch == '\0') {
      break;
    }
  } while (1);
  return iter;
}

i32 string_cmp(const char* s0, const char* s1) {
  return string_n_cmp(s0, s1, UINT32_MAX);
}

i32 string_n_cmp(const char* s0, const char* s1, u32 size) {
  if (!s0) {
    return -1;
  }
  if (!s1) {
    return 1;
  }

  i32 result = 0;
  char* it0 = (char*)&s0[0];
  char* it1 = (char*)&s1[0];
  char left = 0;
  char right = 0;
  u32 iterations = 0;

  do {
    left = *it0++;
    right = *it1++;
    if (left < right) {
      result = -1;
      break;
    }
    else if (left > right) {
      result = 1;
      break;
    }
    ++iterations;
  } while (left && right && iterations < size);
  return result;
}
