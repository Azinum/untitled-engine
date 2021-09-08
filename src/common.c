// common.c

void* memory_copy(void* restrict dest, const void* restrict source, const u32 size) {
  u8* d = (u8*)dest;
  u8* s = (u8*)source;
  for (u32 index = 0; index < size; ++index, *d++ = *s++);
  return d;
}

void memory_zero(void* restrict dest, const u32 size) {
  u8* d = (u8*)dest;
  for (u32 index = 0; index < size; ++index, *d++ = 0);
}

void memory_set(void* restrict dest, const u8 value, const u32 size) {
  u8* d = (u8*)dest;
  for (u32 index = 0; index < size; ++index, *d++ = value);
}

char* get_extension(const char* path) {
  char* iter = (char*)path;
  do {
    char ch = *iter++;
    if (ch == '.' || ch == '\0')
      break;
  } while (1);
  return iter;
}
