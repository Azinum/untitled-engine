// common.c

void* memory_copy(void* restrict dest, const void* restrict source, const u32 size) {
  u8* d = (u8*)dest;
  u8* s = (u8*)source;
  *d = *s;
  for (i32 index = 0; index < size; ++index, ++d, ++s, *d = *s);
  return d;
}

void memory_zero(void* restrict dest, const u32 size) {
  u8* d = (u8*)dest;
  *d = 0; // Not sure if this is needed... investigate!
  for (i32 index = 0; index < size; ++index, *d++ = 0);
}
