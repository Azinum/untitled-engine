// zone.c
// Basic zone memory allocator

#define ALIGNMENT 16
#define ALIGN(n) ((n % ALIGNMENT) ? (n + ALIGNMENT - (n % ALIGNMENT)) : n)

typedef struct Zone {
  u8* data;
  u32 size;
  u32 total_alloc;
} Zone;

Zone zone;

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
  u32 size;
} __attribute__((packed, aligned(ALIGNMENT))) Block_header;

#define ZONE_MIN_SIZE (u32)sizeof(Block_header)

static void zone_write(u32 location, void* data, u32 size);
static i32 zone_fetch_block_header(void* p, Block_header* header);
static void* zone_allocate_block(u32 size, Block_tag block_tag);

void zone_write(u32 location, void* data, u32 size) {
  if (location + size < zone.size) {
    memory_copy(&zone.data[location], data, size);
  }
  else {
    assert("zone_write: Write outside memory zone" && 0);
  }
}

i32 zone_fetch_block_header(void* p, Block_header* header) {
  *header = *((Block_header*)p - 1);
  return NO_ERR;
}

void* zone_allocate_block(u32 size, Block_tag block_tag) {
  void* data = NULL;

  u32 location = 0;
  u32 size_total = 0;
  u8 byte = 0;
  for (u32 index = 0; index < zone.size;) {
begin:
    byte = zone.data[index];
    switch (byte) {
      case TAG_BLOCK_USED: {
        Block_header* header = (Block_header*)&zone.data[index];
        index += header->size + sizeof(Block_header);
        size_total = 0;
        location = index;
        goto begin;
      }
      case TAG_BLOCK_FREE: {
        Block_header* header = (Block_header*)&zone.data[index];
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
      Block_header* header = (Block_header*)&zone.data[location];
      header->tag = block_tag;
      header->size = size;
      data = &zone.data[location + sizeof(Block_header)];
      u32 diff = size_total - (size + sizeof(Block_header));
      if (diff > 0) {
        Block_header next = (Block_header) {
          .tag = TAG_BLOCK_FREE,
          .size = diff - sizeof(Block_header),
        };
        u32 next_location = location + header->size + sizeof(Block_header);
        zone_write(next_location, &next, sizeof(Block_header));
      }
      goto done;
    }
  }
done:
  if (!data) {
    fprintf(stderr, "zone_malloc: Not enough memory in zone memory region\n");
  }
  else {
    zone.total_alloc += size;
  }

  return data;
}

i32 zone_memory_init(u32 size) {
  if (size < ZONE_MIN_SIZE) {
    fprintf(stderr, "zone_memory_init: Zone size must be at least %u bytes (got %u)\n", (u32)ZONE_MIN_SIZE, size);
    return ERR;
  }
  size = ALIGN(size);
  zone.data = m_calloc(size, 1);
  if (!zone.data) {
    return ERR;
  }
  zone.size = size;

  Block_header zone_block = (Block_header) {
    .tag = TAG_BLOCK_FREE,
    .size = size - sizeof(Block_header),
  };
  assert(zone_block.size < zone.size);
  zone_write(0, &zone_block, sizeof(Block_header));
  return NO_ERR;
}

void zone_print_all(FILE* fp) {
  u8 byte = 0;
  Block_header* header = NULL;

  fprintf(fp, "Zone memory: %u/%u bytes (%.3g%%)\n", zone.total_alloc, zone.size, ((float)zone.total_alloc / zone.size * 100.0f));

  for (u32 index = 0; index < zone.size;) {
    byte = zone.data[index];
    switch (byte) {
      case TAG_BLOCK_FREE: {
        header = (Block_header*)&zone.data[index];
        fprintf(fp, "  block (%s) at %9u, %p, size: %9u bytes, %10.4g MB\n", block_tag_info[header->tag], index, (void*)header, header->size, (f32)header->size / MB(1));
        index += header->size + sizeof(Block_header);
        break;
      }
      case TAG_BLOCK_USED: {
        header = (Block_header*)&zone.data[index];
        fprintf(fp, "  block (%s) at %9u, %p, size: %9u bytes, %10.4g MB\n", block_tag_info[header->tag], index, (void*)header, header->size, (f32)header->size / MB(1));
        index += header->size + sizeof(Block_header);
        break;
      }
      default:
        ++index;
        break;
    }
  }
}

void* zone_malloc(u32 size) {
  void* data = NULL;
  size = ALIGN(size);

  data = zone_allocate_block(size, TAG_BLOCK_USED);

  return data;
}

void* zone_calloc(u32 count, u32 size) {
  void* p = zone_malloc(count * size);
  if (p) {
    memory_set(p, 0, count * size);
  }
  return p;
}

void* zone_realloc(void* p, u32 new_size) {
  assert(p);
  void* p_new = zone_malloc(new_size);
  if (p_new) {
    Block_header p_block = {0};
    zone_fetch_block_header(p, &p_block);
    assert(p_block.tag == TAG_BLOCK_USED);
    memory_copy(p_new, p, p_block.size);
    zone_free(p);
  }
  return p_new;
}

u32 zone_free(void* p) {
  if (!p) {
    fprintf(stderr, "zone_free: Tried to free a NULL pointer\n");
    return 0;
  }
  u32 location = (u8*)p - zone.data - sizeof(Block_header);
  Block_header* header = (Block_header*)&zone.data[location];
  if (header->tag != TAG_BLOCK_USED || header->size > zone.size) {  // Sanity check
    fprintf(stderr, "zone_free: Failed to free zone block; corrupted block header (at %u, %p)\n", location, (void*)header);
    return 0;
  }
  header->tag = TAG_BLOCK_FREE;
  memory_zero(p, header->size);
  zone.total_alloc -= header->size;
  return header->size;
}

u32 zone_try_free(void* p) {
  if (p) {
    return zone_free(p);
  }
  return 0;
}

u32 zone_total_alloc() {
  return zone.total_alloc;
}

void zone_dump(const char* path) {
  FILE* fp = fopen(path, "w");
  if (!fp) {
    return;
  }
  fwrite(zone.data, zone.size, 1, fp);
  fclose(fp);
}

void zone_memory_free() {
  if (zone.data) {
    if (zone.total_alloc != 0) {
      fprintf(stderr, "zone_memory_free: Memory leak!\n");
    }
    m_free(zone.data, zone.size);
    zone.data = NULL;
    zone.size = 0;
  }
}
