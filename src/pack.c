// pack.c
// Pack multiple files into a single file
//
// File structure will be as following:
//    PACK_HEADER | LIST OF FILE HEADERS | FILE DATA
//

#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>

#define PACK_HEADER_MAGIC 0x1234aacc
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

typedef struct Pack_header {
  u32 magic;
  u32 root; // Location of root directory
} __attribute__((packed, aligned(STRUCT_ALIGNMENT))) Pack_header;

typedef struct File_header {
  u32 size;
  u32 location;
  u32 compression;  // NOTE: Unused for now
  char name[FILE_NAME_LENGTH];
} __attribute__((packed, aligned(STRUCT_ALIGNMENT))) File_header;

typedef struct Pack_state {
  i32 fd;
  u32 cursor;
} Pack_state;

static i32 pack_state_init(Pack_state* p, const char* path);
static void pack_state_free(Pack_state* p);
static i32 pack_filter(const char* name);
static u32 pack(Pack_state* p, char* path, char* file_name, DIR* dir);
static i32 pack_write(Pack_state* p, const void* data, u32 size);
static i32 pack_write_at(Pack_state* p, const void* data, u32 size, u32 location);

i32 pack_state_init(Pack_state* p, const char* path) {
  i32 result = NO_ERR;
  i32 flags = O_CREAT | O_RDWR | O_SYNC;
  i32 fd = open(path, flags, 0664); // rw, rw, r
  if (fd < 0) {
    fprintf(stderr, "pack_state_init: Failed to open file %s\n", path);
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

u32 pack(Pack_state* p, char* path, char* file_name, DIR* dir) {
  File_header file_header = (File_header) {
    .size = 0,
    .location = 0,
  };
  memory_copy(file_header.name, file_name, string_len(file_name) + 1);  // Include NULL-terminator
  u32 header_location = p->cursor;
  pack_write(p, &file_header, sizeof(File_header));

  if (dir) {
#define DEFAULT_FILE_COUNT 32
    struct {
      u32* data;
      u32 count;
      u32 size;
    } dir_entries = {
      .data = zone_malloc(DEFAULT_FILE_COUNT * sizeof(u32)),
      .count = 0,
      .size = DEFAULT_FILE_COUNT,
    };

    u32 file_count = 0;
    struct dirent* entry = NULL;
    while ((entry = readdir(dir)) != NULL) {
      const char* name = entry->d_name;
      if (pack_filter(name)) continue;

      u32 entry_header_location = 0;
      char entry_path[PATH_SIZE_MAX] = {0};
      snprintf(entry_path, PATH_SIZE_MAX, "%s/%s", path, entry->d_name);
      DIR* entry_dir = opendir(entry_path);

      if (entry_dir) {
        entry_header_location = pack(p, entry_path, entry->d_name, entry_dir);
        closedir(entry_dir);
      }
      else {
        entry_header_location = pack(p, entry_path, entry->d_name, NULL);
      }
      file_count++;

      if (dir_entries.count >= dir_entries.size) {
        void* tmp = zone_realloc(dir_entries.data, dir_entries.size * 2 * sizeof(u32));
        if (tmp) {
          dir_entries.data = (u32*)tmp;
          dir_entries.size *= 2;
        }
      }
      dir_entries.data[dir_entries.count++] = entry_header_location;
    }
    file_header.location = p->cursor;
    for (u32 i = 0; i < dir_entries.count; ++i) {
      file_header.size += pack_write(p, &dir_entries.data[i], sizeof(u32));
    }
    zone_try_free(dir_entries.data);
  }
  else {
    Buffer file_contents = {0};
    if (read_file(path, &file_contents) == NO_ERR) {
      if (file_contents.size > 0) {
        file_header.location = p->cursor;
        file_header.size += pack_write(p, &file_contents.data[0], file_contents.size);
        buffer_free(&file_contents);
      }
    }
  }

  pack_write_at(p, &file_header, sizeof(File_header), header_location);
  return header_location;
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
