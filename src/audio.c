// audio.c

i32 audio_load_from_pack(const char* path, const char* pack_file, Audio_source* audio) {
  i32 result = NO_ERR;
  Buffer buffer = {0};
  if ((result = read_pack_file(path, pack_file, &buffer)) == NO_ERR) {
    char* ext = get_extension(path);
    if (!string_cmp(ext, "ogg")) {
      result = audio_vorbis_load_from_buffer(path, &buffer, audio);
    }
    else {
      fprintf(stderr, "audio_load_from_pack: Audio file format `%s` not supported for file `%s`\n", ext, path);
      result = ERR;
    }
    buffer_free(&buffer);
  }
  return result;
}

void audio_unload(Audio_source* audio) {
  assert(audio != NULL);
  if (audio->buffer) {
    zone_free(audio->buffer);
    audio->buffer = NULL;
    memory_zero(audio, sizeof(Audio_source));
  }
}

i32 audio_convert_to_f32_buffer(f32* restrict out_buffer, i16* restrict in_buffer, u32 sample_count) {
  for (u32 sample_index = 0; sample_index < sample_count; ++sample_index) {
    *out_buffer++ = in_buffer[sample_index] / ((f32)INT16_MAX);
  }
  return NO_ERR;
}

i32 audio_convert_to_i16_buffer(i16* restrict out_buffer, f32* restrict in_buffer, u32 sample_count) {
  for (u32 sample_index = 0; sample_index < sample_count; ++sample_index) {
    *out_buffer++ = in_buffer[sample_index] * INT16_MAX;
  }
  return NO_ERR;
}

void audio_zero_f32_buffer(f32* buffer, u32 size) {
  if (!buffer) {
    return;
  }
#if USE_SSE
  if (!(size % sizeof(__m128))) {
    __m128 zero = _mm_set1_ps(0.0f);
    __m128* dest = (__m128*)buffer;
    u32 chunk_size = sizeof(__m128);
    u32 max_chunk = size / chunk_size;
    for (u32 chunk_index = 0; chunk_index < max_chunk; ++chunk_index, ++dest) {
      *dest = zero;
    }
    return;
  }
#endif
  memset(buffer, 0, size);
}

void audio_sum_f32_buffers(f32* out_buffer, f32* in_a, f32* in_b, u32 size) {
#if USE_SSE
  assert(!(size % sizeof(__m128)) && "size of buffers must be a multiple of 16");
  __m128* dest = (__m128*)out_buffer;
  __m128* a = (__m128*)in_a;
  __m128* b = (__m128*)in_b;
  u32 chunk_size = sizeof(__m128);
  u32 max_chunk = size / chunk_size;
  for (u32 chunk_index = 0; chunk_index < max_chunk; ++chunk_index, ++dest, ++a, ++b) {
    *dest = _mm_add_ps(*a, *b);
  }
#else
  f32* dest = out_buffer;
  f32* a = in_a;
  f32* b = in_b;
  u32 chunk_size = sizeof(f32);
  u32 max_chunk = size / chunk_size;
  for (u32 chunk_index = 0; chunk_index < max_chunk; ++chunk_index, ++dest, ++a, ++b) {
    *dest = *a + *b;
  }
#endif
}
