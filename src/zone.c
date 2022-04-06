// zone.c
// Basic zone memory allocator

#define ALIGNMENT 16
#define ALIGN(n) ((n % ALIGNMENT) ? (n + ALIGNMENT - (n % ALIGNMENT)) : n)
#define SWEEP_ON_FREE 1

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
  z->total_alloc = 0;

  Block_header zone_block = (Block_header) {
    .tag = TAG_BLOCK_FREE,
    .size = size,
  };
  assert(zone_block.size <= z->size);
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
  size += sizeof(Block_header);

  if (z->total_alloc + size >= z->size) {
    goto done;
  }

  size_t location = 0;
  size_t size_total = 0;
  Block_header* header = NULL;
  for (size_t index = 0; index < z->size;) {
begin:
    header = (Block_header*)&z->data[index];
    switch (header->tag) {
      case TAG_BLOCK_USED: {
        index += header->size;
        size_total = 0;
        location = index;
        goto begin;
      }
      case TAG_BLOCK_FREE: {
        size_total += header->size;
        index += header->size;
        break;
      }
      default: {
        assert("something went very wrong" && 0);
        return NULL;
      }
    }
    if (size_total >= size) {
      Block_header* header = (Block_header*)&z->data[location];
      header->tag = block_tag;
      header->size = size;
      data = (u8*)header + sizeof(Block_header);
      size_t diff = size_total - header->size;
      if (diff > 0) {
        Block_header next = (Block_header) {
          .tag = TAG_BLOCK_FREE,
          .size = diff,
        };
        size_t next_location = location + header->size;
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

i32 zone_memory_init(size_t size) {
  zone_init(&zones[ZONE_DEFAULT], size);
  zone_init(&zones[ZONE_TEMP], MB(2));
  return NO_ERR;
}

void zone_print_all(FILE* fp) {
  Block_header* header = NULL;

  size_t which_zone = zone - &zones[0];

  fprintf(fp, "Zone memory (%lu): %lu/%lu bytes, %lu/%lu MB (%.3g%%)\n", which_zone, zone->total_alloc, zone->size, zone->total_alloc / MB(1), zone->size / MB(1), ((f32)zone->total_alloc / zone->size * 100.0f));
  for (size_t index = 0; index < zone->size;) {
    header = (Block_header*)&zone->data[index];
    fprintf(fp, "  block (%s) at %9lu to %9lu, %p, size: %9lu bytes, %10.4g MB\n", block_tag_info[header->tag], index, index + header->size, (void*)header, header->size, (f32)header->size / MB(1));
    index += header->size;
  }
}

i32 zone_validate() {
  size_t size_total = 0;
  Zone* z = zone;
  if (!z) {
    return 0;
  }
  for (size_t index = 0; index < z->size;) {
    Block_header* header = (Block_header*)&z->data[index];
    size_total += header->size;
    index += header->size;
    if (header->tag != TAG_BLOCK_USED && header->tag != TAG_BLOCK_FREE) {
      fprintf(stderr, "Bad header tag at: %p, %u, size: %lu\n", (void*)header, header->tag, header->size);
      return 0;
    }
  }
  return size_total == z->size;
}

void zone_sweep() {
  Zone* z = zone;

  size_t size_total = 0;
  size_t location = 0;

  for (size_t index = 0; index < z->size;) {
    Block_header* header = (Block_header*)&z->data[index];
    Block_header* start = (Block_header*)&z->data[location];
    size_t next_size = size_total + header->size;
    index += header->size;

    if (header->tag == TAG_BLOCK_FREE) {
      size_total = next_size;
    }
    // Did we reach end or a used block?
    if (header->tag == TAG_BLOCK_USED || index >= z->size) {
      if (start->tag == TAG_BLOCK_FREE) {
        start->size = size_total;
      }
      location = index;
      size_total = 0;
    }
  }
}

void* zone_malloc(size_t size) {
  void* data = NULL;
  size = ALIGN(size);

  assert(size != 0);

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
    memory_copy(p_new, p, p_block.size - sizeof(Block_header));
    zone_free(p);
  }
  return p_new;
}

size_t zone_free(void* p) {
  if (!p) {
    return 0;
  }

  size_t location = ((u8*)p - sizeof(Block_header)) - &zone->data[0];
  Block_header* header = (Block_header*)&zone->data[location];

  if (header->tag != TAG_BLOCK_USED || location + header->size > zone->size) {  // Sanity check
    fprintf(stderr, "zone_free: Failed to free zone block; corrupted block header (at %lu, %p)\n", location, (void*)header);
    return 0;
  }
  header->tag = TAG_BLOCK_FREE;
  zone->total_alloc -= header->size;
#if SWEEP_ON_FREE
  zone_sweep();
#endif
  return header->size;
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
  FILE* fp = fopen(path, "wb");
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
