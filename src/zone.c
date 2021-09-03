// zone.c
// Basic zone memory allocator

#define ALIGNMENT 16
#define ALIGN(n) ((n % ALIGNMENT) ? (n + ALIGNMENT - (n % ALIGNMENT)) : n)

struct {
  u8* data;
  u32 size;
  u32 total_alloc;
} zone;

enum Block_tag {
  TAG_BLOCK_USED = 1,
  TAG_BLOCK_FREE
};

typedef struct Block_header {
  u8 tag;
  u32 size;
} __attribute__((packed, aligned(ALIGNMENT))) Block_header;

#define ZONE_MIN_SIZE (u32)sizeof(Block_header)

static void zone_write(u32 location, void* data, u32 size);

void zone_write(u32 location, void* data, u32 size) {
  if (location + size < zone.size) {
    memory_copy(&zone.data[location], data, size);
  }
  else {
    assert(0 && "write outside memory zone");
  }
}

i32 zone_memory_init(u32 size) {
  if (size < ZONE_MIN_SIZE) {
    fprintf(stderr, "Zone size must be at least %u bytes\n", (u32)ZONE_MIN_SIZE);
    return ERR;
  }
  zone.data = m_calloc(size, 1);
  if (!zone.data) {
    return ERR;
  }
  zone.size = size;

  Block_header zone_block = (Block_header) {
    .tag = TAG_BLOCK_FREE,
    .size = size - sizeof(Block_header),
  };
  zone_write(0, &zone_block, sizeof(Block_header));
  return NO_ERR;
}

void* zone_malloc(u32 size) {
  void* data = NULL;
  size = ALIGN(size);

  u32 location = 0;
  u32 size_total = 0;
  u8 byte = 0;
  for (u32 index = 0; index < zone.size; ++index) {
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
        ++size_total;
        break;
      }
    }
    if (size_total >= size) {
      Block_header* header = (Block_header*)&zone.data[location];
      header->tag = TAG_BLOCK_USED;
      header->size = size;
      data = &zone.data[location + sizeof(Block_header)];
      memory_zero(data, size);
      // printf("We found enough memory! At: %u (%lu)\n", location, location + sizeof(Block_header));
      goto done;
    }
  }
done:
  if (!data) {
    fprintf(stderr, "Not enough memory in zone memory region\n");
  }
  else {
    zone.total_alloc += size;
  }

  return data;
}

u32 zone_free(void* p) {
  u32 location = (u8*)p - zone.data - sizeof(Block_header);
  Block_header* header = (Block_header*)&zone.data[location];
  if (header->tag != TAG_BLOCK_USED || header->size > zone.size) {  // Sanity check
    fprintf(stderr, "Failed to free zone block; corrupted block header (at %u)\n", location);
    return 0;
  }
  header->tag = TAG_BLOCK_FREE;
  memory_zero(p, header->size);
  zone.total_alloc -= header->size;
  return header->size;
}

u32 zone_total_alloc() {
  return zone.total_alloc;
}

void zone_memory_free() {
  if (zone.data) {
    m_free(zone.data, zone.size);
    zone.data = NULL;
    zone.size = 0;
  }
}
