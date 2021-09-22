// pack.c
// Hierarchical file pack/unpacker
//

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define PACK_HEADER_MAGIC 0xffff0001
#define STRUCT_ALIGNMENT 4
#define FILE_NAME_LENGTH 32
#define PATH_SIZE_MAX 512

const char* filter_ext_files[] = {
  "xcf",
};

const char* filter_files[] = {
  ".",
  "..",
};

const char* file_type_desc[] = {
  "unspecified",
  "default",
  "directory",
};

enum File_type {
  FILE_TYPE_UNSPECIFIED = 0,
  FILE_TYPE_DEFAULT,
  FILE_TYPE_DIR,

  MAX_FILE_TYPE,
};

typedef struct Dir_entry {
  u32 location;
  u32 name_hash;
} __attribute__((packed, aligned(STRUCT_ALIGNMENT))) Dir_entry;

typedef struct Pack_header {
  u32 magic;
  Dir_entry root;
} __attribute__((packed, aligned(STRUCT_ALIGNMENT))) Pack_header;

typedef struct File_header {
  u32 size;
  u32 location;
  u16 type;
  u16 compression;  // NOTE: Unused for now
  char name[FILE_NAME_LENGTH];
} __attribute__((packed, aligned(STRUCT_ALIGNMENT))) File_header;

typedef struct Pack_state {
  i32 fd;
  u32 cursor;
} Pack_state;

static i32 pack_state_init(Pack_state* p, const char* path);
static void pack_state_free(Pack_state* p);
static i32 pack_filter(const char* name);
static Dir_entry pack(Pack_state* p, char* path, char* file_name, DIR* dir);
static i32 pack_write(Pack_state* p, const void* data, u32 size);
static i32 pack_write_at(Pack_state* p, const void* data, u32 size, u32 location);

static i32 pack_read(Pack_state* p, void* data, u32 size);
static i32 pack_read_at(Pack_state* p, void* data, u32 size, u32 location);
static i32 pack_iterate_path(Pack_state* p, const char* path, File_header current, File_header* header);
static i32 pack_find_file_header(Pack_state* p, Dir_entry* root, const char* path, File_header* header);
static void pack_print_hierarchy(Pack_state* p, Dir_entry* dir_entry, u32 level, FILE* fp);

i32 pack_state_init(Pack_state* p, const char* path) {
  i32 result = NO_ERR;
  i32 flags = O_CREAT | O_RDWR | O_SYNC;
  i32 fd = open(path, flags, 0664); // rw, rw, r
  if (fd < 0) {
    fprintf(stderr, "pack_state_init: Failed to open file '%s'\n", path);
    return ERR;
  }
  p->fd = fd;
  p->cursor = 0;
  return result;
}

void pack_state_free(Pack_state* p) {
  if (p->fd >= 0) {
    close(p->fd);
    p->fd = -1;
  }
}

i32 pack_filter(const char* name) {
  for (u32 i = 0; i < ARR_SIZE(filter_files); ++i) {
    if (!string_cmp(name, filter_files[i])) {
      return 1;
    }
  }
  for (u32 i = 0; i < ARR_SIZE(filter_ext_files); ++i) {
    if (!string_cmp(get_extension(name), filter_ext_files[i])) {
      return 1;
    }
  }
  return 0;
}

Dir_entry pack(Pack_state* p, char* path, char* file_name, DIR* dir) {
  File_header file_header = (File_header) {
    .size = 0,
    .location = 0,
    .type = FILE_TYPE_UNSPECIFIED,
    .compression = 0,
  };
  memory_copy(file_header.name, file_name, string_len(file_name) + 1);  // Include NULL-terminator
  u32 header_location = p->cursor;
  pack_write(p, &file_header, sizeof(File_header));

  if (dir) {
#define DEFAULT_FILE_COUNT 32
    struct {
      Dir_entry* data;
      u32 count;
      u32 size;
    } dir_entries = {
      .data = zone_malloc(DEFAULT_FILE_COUNT * sizeof(Dir_entry)),
      .count = 0,
      .size = DEFAULT_FILE_COUNT,
    };

    u32 file_count = 0;
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
      const char* name = entry->d_name;
      if (pack_filter(name)) continue;

      char entry_path[PATH_SIZE_MAX] = {0};
      snprintf(entry_path, PATH_SIZE_MAX, "%s/%s", path, entry->d_name);
      DIR* entry_dir = opendir(entry_path);

      Dir_entry dir_entry;
      if (entry_dir) {
        dir_entry = pack(p, entry_path, entry->d_name, entry_dir);
        closedir(entry_dir);
      }
      else {
        dir_entry = pack(p, entry_path, entry->d_name, NULL);
      }
      file_count++;

      if (dir_entries.count >= dir_entries.size) {
        u32 new_size = 2 * dir_entries.size;
        void* tmp = zone_realloc(dir_entries.data, new_size * sizeof(Dir_entry));
        if (tmp) {
          dir_entries.data = (Dir_entry*)tmp;
          dir_entries.size = new_size;
        }
      }
      dir_entries.data[dir_entries.count++] = dir_entry;
    }
    file_header.location = p->cursor;
    file_header.type = FILE_TYPE_DIR;
    for (u32 i = 0; i < dir_entries.count; ++i) {
      file_header.size += pack_write(p, &dir_entries.data[i], sizeof(Dir_entry));
    }
    zone_free(dir_entries.data);
  }
  else {
    Buffer file_contents = {0};
    if (read_file(path, &file_contents) == NO_ERR) {
      if (file_contents.size > 0) {
        file_header.location = p->cursor;
        file_header.size += pack_write(p, &file_contents.data[0], file_contents.size);
        file_header.type = FILE_TYPE_DEFAULT;
        buffer_free(&file_contents);
      }
    }
  }

  pack_write_at(p, &file_header, sizeof(File_header), header_location);
  return (Dir_entry) {
    .location = header_location,
    .name_hash = hash(file_header.name, FILE_NAME_LENGTH),
  };
}

i32 pack_write(Pack_state* p, const void* data, u32 size) {
  p->cursor += size;
  return write(p->fd, data, size);
}

i32 pack_write_at(Pack_state* p, const void* data, u32 size, u32 location) {
  lseek(p->fd, location, SEEK_SET);
  i32 result = write(p->fd, data, size);
  lseek(p->fd, p->cursor, SEEK_SET);
  return result;
}

i32 pack_read(Pack_state* p, void* data, u32 size) {
  p->cursor += size;
  return read(p->fd, data, size);
}

i32 pack_read_at(Pack_state* p, void* data, u32 size, u32 location) {
  lseek(p->fd, location, SEEK_SET);
  i32 result = read(p->fd, data, size);
  lseek(p->fd, p->cursor, SEEK_SET);
  return result;
}

i32 pack_iterate_path(Pack_state* p, const char* path, File_header current, File_header* header) {
  i32 result = ERR;
  u8 end = 0;
  char name[FILE_NAME_LENGTH] = {0};
  u32 length = string_copy_filename_in_path(name, path, &end);
  u32 name_hash = hash(name, FILE_NAME_LENGTH);
  if (current.type == FILE_TYPE_DIR) {
    Dir_entry* entries = zone_malloc(current.size);
    if (entries) {
      pack_read_at(p, entries, current.size, current.location);
      u32 entry_count = current.size / sizeof(Dir_entry);
      u8 found_entry = 0;
      Dir_entry entry = {0};
      for (u32 entry_index = 0; entry_index < entry_count; ++entry_index) {
        entry = entries[entry_index];
        if (name_hash == entry.name_hash) {
          found_entry = 1;
          break;
        }
      }
      zone_free(entries);
      if (found_entry) {
        File_header next;
        pack_read_at(p, &next, sizeof(File_header), entry.location);
        if (end) {
          *header = next;
          return NO_ERR;
        }
        return pack_iterate_path(p, path + length, next, header);
      }
    }
  }
  else {
    return ERR;
  }
  return result;
}

i32 pack_find_file_header(Pack_state* p, Dir_entry* root, const char* path, File_header* header) {
  File_header root_header;
  pack_read_at(p, &root_header, sizeof(File_header), root->location);

  u8 end = 0;
  u32 length = string_filename_in_path(path, &end);
  return pack_iterate_path(p, path + length, root_header, header);
}

void pack_print_hierarchy(Pack_state* p, Dir_entry* dir_entry, u32 level, FILE* fp) {
  assert(fp);

  File_header header = {0};
  pack_read_at(p, &header, sizeof(File_header), dir_entry->location);

  for (u32 tab = 0; tab < level; ++tab) {
    fprintf(fp, "  ");
  }

  switch (header.type) {
    case FILE_TYPE_DEFAULT: {
      fprintf(fp, "%s\n", header.name);
      break;
    }
    case FILE_TYPE_DIR: {
      fprintf(fp, "%s/\n", header.name);
      Dir_entry* entries = zone_malloc(header.size);
      u32 entry_count = header.size / sizeof(Dir_entry);
      if (pack_read_at(p, entries, header.size, header.location) == header.size) {
        for (u32 entry_index = 0; entry_index < entry_count; ++entry_index) {
          pack_print_hierarchy(p, &entries[entry_index], level + 1, fp);
        }
      }
      else {
        fprintf(stderr, "Failed to read contents of file '%s'\n", header.name);
      }
      zone_free(entries);
      break;
    }
    default:
      break;
  }
}


i32 pack_dir(const char* dest, const char* source) {
  i32 result = NO_ERR;
  Pack_state pack_state = {0};
  pack_state_init(&pack_state, dest);

  Pack_header header = {
    .magic = PACK_HEADER_MAGIC,
  };
  pack_write(&pack_state, &header, sizeof(Pack_header));

  char path[PATH_SIZE_MAX] = {0};
  snprintf(path, PATH_SIZE_MAX, "%s", source);
  DIR* root_dir = opendir(source);
  if (root_dir) {
    header.root = pack(&pack_state, path, path, root_dir);
    closedir(root_dir);
  }
  pack_write_at(&pack_state, &header, sizeof(Pack_header), 0);
  pack_state_free(&pack_state);
  return result;
}

i32 unpack(const char* path) {
  i32 result = NO_ERR;
  Pack_state pack_state = {0};
  pack_state_init(&pack_state, path);

  Pack_header header = {0};
  pack_read(&pack_state, &header, sizeof(Pack_header));
  if (header.magic == PACK_HEADER_MAGIC) {
    pack_print_hierarchy(&pack_state, &header.root, 0, stdout);
  }
  else {
    fprintf(stderr, "Invalid magic constant in file '%s'\n", path);
    result = ERR;
  }

  pack_state_free(&pack_state);
  return result;
}


// NOTE(lucas): Maybe keep pack file open so that we don't have to
// re-open it multiple times during runtime of the program?
i32 read_pack_file(const char* path, const char* pack_file, Buffer* buffer) {
  assert(path && pack_file);
  i32 result = NO_ERR;

  Pack_state pack_state = {0};
  if ((result = pack_state_init(&pack_state, pack_file)) == NO_ERR) {
    Pack_header header = {0};
    pack_read(&pack_state, &header, sizeof(Pack_header));
    if (header.magic == PACK_HEADER_MAGIC) {
      File_header file_header;
      if ((result = pack_find_file_header(&pack_state, &header.root, path, &file_header)) == NO_ERR) {
        buffer->data = zone_malloc(file_header.size);
        if (buffer->data) {
          pack_read_at(&pack_state, &buffer->data[0], file_header.size, file_header.location);
          buffer->size = file_header.size;
        }
        else {
          result = ERR;
        }
      }
      else {
        fprintf(stderr, "read_pack_file: File '%s' does not exist in pack '%s'\n", path, pack_file);
        result = ERR;
      }
    }
    else {
      fprintf(stderr, "read_pack_file: Invalid magic constant in file '%s'\n", pack_file);
      result = ERR;
    }
    pack_state_free(&pack_state);
  }
  return result;
}
