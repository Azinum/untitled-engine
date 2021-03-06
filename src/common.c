// common.c

void buffer_free(Buffer* buffer) {
  assert(buffer);
  if (buffer->data) {
    zone_free(buffer->data);
    buffer->data = NULL;
    buffer->size = 0;
  }
}

u32 buffer_iterate(void* restrict dest, Buffer* source, u32 size, u32* iter) {
  assert("buffer_iterate: write outside buffer memory area" && (*iter + size) <= source->size);
  memory_copy(dest, &source->data[*iter], size);

  *iter += size;

  return size;
}

void* memory_copy(void* restrict dest, const void* restrict source, const u32 size) {
#if USE_SSE
  if (!(size % sizeof(__m128))) {
    __m128* d4 = (__m128*)dest;
    __m128* s4 = (__m128*)source;
    const u32 chunk_count = size / sizeof(__m128);
    for (u32 index = 0; index < chunk_count; ++index) {
      _mm_store_ps((f32*)d4++, *s4++);
    }
    return dest;
  }
#endif
  u8* d = (u8*)dest;
  u8* s = (u8*)source;
  for (u32 index = 0; index < size; ++index, *d++ = *s++);
  return dest;
}

void memory_zero(void* restrict dest, const u32 size) {
  memory_set(dest, 0, size);
}

void memory_set(void* restrict dest, const u8 value, const u32 size) {
#if USE_SSE && 0
  // FIXME: SIMD badness!!
  if (!(size % sizeof(__m128))) {
    __m128i* d4 = (__m128i*)dest;
    __m128i v = _mm_set1_epi8(value);
    const u32 chunk_count = size / sizeof(__m128);
    for (u32 index = 0; index < chunk_count; ++index) {
      _mm_stream_si128(d4++, v);
    }
    return;
  }
#endif
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

i32 string_len(const char* s) {
  if (!s) {
    return 0;
  }
  i32 length = 0;
  char* iter = (char*)s;
  while (*iter++ != '\0') {
    ++length;
  }
  return length;
}

i32 string_copy(char* restrict dest, const char* restrict source) {
  char* it0 = dest;
  char* it1 = (char*)source;
  if (!it0 || !it1) {
    return 0;
  }
  while (*it1 != '\0') {
    *it0++ = *it1++;
  }
  return 0;
}

u32 string_copy_filename_in_path(char* restrict dest, const char* restrict path, u8* end) {
  u32 length = 0;
  char* it0 = dest;
  char* it1 = (char*)path;

  do {
    ++length;
    if (*it1 == '\0') {
      if (end) {
        *end = 1;
      }
      break;
    }
    else if (*it1 == '/') {
      break;
    }
    *it0++ = *it1++;
  } while (1);

  *it0 = '\0';
  return length;
}

u32 string_filename_in_path(const char* path, u8* end) {
  u32 length = 0;
  char* iter = (char*)path;

  do {
    ++length;
    if (*iter == '\0') {
      if (end) {
        *end = 1;
      }
      break;
    }
    else if (*iter == '/') {
      break;
    }
    ++iter;
  } while (1);
  return length;
}

i32 read_file(const char* path, Buffer* buffer) {
	i32 result = NO_ERR;
	u32 num_bytes_read = 0;

	FILE* fp = fopen(path, "rb");
	if (!fp) {
		fprintf(stderr, "read_file: File '%s' does not exist\n", path);
		return ERR;
	}

	fseek(fp, 0, SEEK_END);
	u32 size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buffer->data = (char*)zone_malloc(size * sizeof(char));
	buffer->size = size;
	if (!buffer->data) {
		buffer->size = 0;
		result = ERR;
		goto done;
	}

	num_bytes_read = fread(buffer->data, 1, size, fp);
	if (num_bytes_read != size) {
		fprintf(stderr, "read_file: Failed to read file '%s'\n", path);
		result = ERR;
		goto done;
	}

done:
	fclose(fp);
	return result;
}

inline u64 read_tsc() {
  return __rdtsc();
}
