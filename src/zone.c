// zone->c
// Basic zone memory allocator
// TODO(lucas): Make this thread safe when we actually have some threaded code

#define ALIGNMENT 16
#define ALIGN(n) ((n % ALIGNMENT) ? (n + ALIGNMENT - (n % ALIGNMENT)) : n)

typedef struct Zone {
  u8* data;
  size_t size;
  size_t total_alloc;
} Zone;

typedef enum Zone_type {
  ZONE_DEFAULT = 0,
  ZONE_TEMP,

  MAX_ZONE,
} Zone_type;

Zone zones[MAX_ZONE] = {0};
Zone* zone = &zones[ZONE_DEFAULT];

typedef enum Block_tag {
  TAG_BLOCK_USED = 1,
  TAG_BLOCK_FREE,

  MAX_BLOCK_TAG,
} Block_tag;

const char* block_tag_info[MAX_BLOCK_TAG] = {
  "none",
  "used",
  "free",
};

typedef struct Block_header {
  u8 tag;
  size_t size;
} __attribute__((packed, aligned(ALIGNMENT))) Block_header;

#define ZONE_MIN_SIZE (size_t)sizeof(Block_header)

static i32 zone_init(Zone* z, size_t size);
static void zone_write(Zone* z, size_t location, void* data, size_t size);
static i32 zone_fetch_block_header(Zone* z, void* p, Block_header* header);
static void* zone_allocate_block(Zone* z, size_t size, Block_tag block_tag);

i32 zone_init(Zone* z, size_t size) {
  if (size < ZONE_MIN_SIZE) {
    fprintf(stderr, "zone_init: Zone size must be at least %lu bytes (got %lu)\n", (size_t)ZONE_MIN_SIZE, size);
    return ERR;
  }
  size = ALIGN(size);
  z->data = m_calloc(size, 1);
  if (!z->data) {
    return ERR;
  }
  z->size = size;

  Block_header zone_block = (Block_header) {
    .tag = TAG_BLOCK_FREE,
    .size = size - sizeof(Block_header),
  };
  assert(zone_block.size < z->size);
  zone_write(z, 0, &zone_block, sizeof(Block_header));
  return NO_ERR;
}

void zone_write(Zone* z, size_t location, void* data, size_t size) {
  if (location + size < z->size) {
    memory_copy(&z->data[location], data, size);
  }
  else {
    assert("zone_write: Write outside memory zone" && 0);
  }
}

i32 zone_fetch_block_header(Zone* z, void* p, Block_header* header) {
  *header = *((Block_header*)p - 1);
  return NO_ERR;
}

void* zone_allocate_block(Zone* z, size_t size, Block_tag block_tag) {
  void* data = NULL;

  size_t location = 0;
  size_t size_total = 0;
  u8 byte = 0;
  for (size_t index = 0; index < z->size;) {
begin:
    byte = z->data[index];
    switch (byte) {
      case TAG_BLOCK_USED: {
        Block_header* header = (Block_header*)&zone->data[index];
        index += header->size + sizeof(Block_header);
        size_total = 0;
        location = index;
        goto begin;
      }
      case TAG_BLOCK_FREE: {
        Block_header* header = (Block_header*)&z->data[index];
        size_total += header->size + sizeof(Block_header);
        index += header->size + sizeof(Block_header);
        break;
      }
      default: {
        assert(0);  // We should not be able to get here
        break;
      }
    }
    if (size_total >= size) {
      Block_header* header = (Block_header*)&z->data[location];
      header->tag = block_tag;
      header->size = size;
      data = &z->data[location + sizeof(Block_header)];
      size_t diff = size_total - (size + sizeof(Block_header));
      if (diff > 0) {
        Block_header next = (Block_header) {
          .tag = TAG_BLOCK_FREE,
          .size = diff - sizeof(Block_header),
        };
        size_t next_location = location + header->size + sizeof(Block_header);
        zone_write(z, next_location, &next, sizeof(Block_header));
      }
      goto done;
    }
  }
done:
  if (!data) {
    fprintf(stderr, "zone_malloc: Not enough memory in zone memory region\n");
  }
  else {
    z->total_alloc += size;
  }

  return data;
}

i32 zone_memory_init(size_t size, size_t temp_size) {
  zone_init(&zones[ZONE_DEFAULT], size);
  zone_init(&zones[ZONE_TEMP], temp_size);
  return NO_ERR;
}

void zone_print_all(FILE* fp) {
  u8 byte = 0;
  Block_header* header = NULL;

  fprintf(fp, "Zone memory: %lu/%lu bytes (%.3g%%)\n", zone->total_alloc, zone->size, ((f32)zone->total_alloc / zone->size * 100.0f));

  for (size_t index = 0; index < zone->size;) {
    byte = zone->data[index];
    switch (byte) {
      case TAG_BLOCK_FREE: {
        header = (Block_header*)&zone->data[index];
        fprintf(fp, "  block (%s) at %9lu, %p, size: %9lu bytes, %10.4g MB\n", block_tag_info[header->tag], index, (void*)header, header->size, (f32)header->size / MB(1));
        index += header->size + sizeof(Block_header);
        break;
      }
      case TAG_BLOCK_USED: {
        header = (Block_header*)&zone->data[index];
        fprintf(fp, "  block (%s) at %9lu, %p, size: %9lu bytes, %10.4g MB\n", block_tag_info[header->tag], index, (void*)header, header->size, (f32)header->size / MB(1));
        index += header->size + sizeof(Block_header);
        break;
      }
      default:
        ++index;
        break;
    }
  }
}

void* zone_malloc(size_t size) {
  void* data = NULL;
  size = ALIGN(size);

  data = zone_allocate_block(zone, size, TAG_BLOCK_USED);

  return data;
}

void* zone_calloc(size_t count, size_t size) {
  void* p = zone_malloc(count * size);
  if (p) {
    memory_set(p, 0, count * size);
  }
  return p;
}

void* zone_realloc(void* p, size_t new_size) {
  assert(p);
  void* p_new = zone_malloc(new_size);
  if (p_new) {
    Block_header p_block = {0};
    zone_fetch_block_header(zone, p, &p_block);
    assert(p_block.tag == TAG_BLOCK_USED);
    memory_copy(p_new, p, p_block.size);
    zone_free(p);
  }
  return p_new;
}

size_t zone_free(void* p) {
  if (!p) {
    fprintf(stderr, "zone_free: Tried to free a NULL pointer\n");
    return 0;
  }
  size_t location = (u8*)p - zone->data - sizeof(Block_header);
  Block_header* header = (Block_header*)&zone->data[location];
  if (header->tag != TAG_BLOCK_USED || header->size > zone->size) {  // Sanity check
    fprintf(stderr, "zone_free: Failed to free zone block; corrupted block header (at %lu, %p)\n", location, (void*)header);
    return 0;
  }
  header->tag = TAG_BLOCK_FREE;
  memory_zero(p, header->size);
  zone->total_alloc -= header->size;
  return header->size;
}

size_t zone_try_free(void* p) {
  if (p) {
    return zone_free(p);
  }
  return 0;
}

void* zone_temp_malloc(size_t size) {
  void* result = NULL;
  Zone* prev = zone;
  zone = &zones[ZONE_TEMP];

  result = zone_malloc(size);

  zone = prev;
  return result;
}

size_t zone_temp_free(void* p) {
  size_t result = 0;
  Zone* prev = zone;
  zone = &zones[ZONE_TEMP];

  result = zone_free(p);

  zone = prev;
  return result;
}

size_t zone_total_alloc() {
  return zone->total_alloc;
}

void zone_dump(const char* path) {
  FILE* fp = fopen(path, "w");
  if (!fp) {
    return;
  }
  fwrite(zone->data, zone->size, 1, fp);
  fclose(fp);
}

void zone_memory_free() {
  for (u32 zone_index = 0; zone_index < MAX_ZONE; ++zone_index) {
    Zone* z = &zones[zone_index];
    if (z->data) {
      if (z->total_alloc != 0) {
        fprintf(stderr, "zone_memory_free: Memory leak!\n");
      }
      m_free(z->data, z->size);
      z->data = NULL;
      z->size = 0;
    }
  }
}
