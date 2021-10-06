// hash.c

hash_t djb2_hash(u8* data, u32 size) {
  hash_t result = 5381;
  if (!data) {
    return result;
  }
  for (u32 i = 0; i < size; ++i) {
    result = ((result << 5) + result) + data[i];
  }
  return result;
}
