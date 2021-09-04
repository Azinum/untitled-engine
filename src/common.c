// common.c

void* memory_copy(void* restrict dest, const void* restrict source, const u32 size) {
  u8* d = (u8*)dest;
  u8* s = (u8*)source;
  for (i32 index = 0; index < size; ++index, *d++ = *s++);
  return d;
}

void memory_zero(void* restrict dest, const u32 size) {
  u8* d = (u8*)dest;
  for (i32 index = 0; index < size; ++index, *d++ = 0);
}

char* get_extension(const char* path) {
  char* iter = (char*)path;
  for (;;) {
    char ch = *iter++;
    if (ch == '.' || ch == '\0')
      break;
  }
  return iter;
}
