// mesh_wavefront.c

#define MAX_LINE_SIZE 256

#define safe_scanf(ScanStatus, Iterator, Format, ...) { \
  u32 num_bytes_read = 0; \
  ScanStatus = sscanf(Iterator, Format "%n", __VA_ARGS__, &num_bytes_read); \
  Iterator += num_bytes_read; \
}

// Prefetch mesh information (number of vertices, normals e.t.c.), and load that information into the mesh itself
static i32 wavefront_prepare_mesh(Buffer* buffer, Mesh* mesh);

i32 wavefront_prepare_mesh(Buffer* buffer, Mesh* mesh) {
  return NO_ERR;
}

i32 wavefront_mesh_load(const char* path, Mesh* mesh) {
  i32 result = NO_ERR;
  Buffer buffer;
  if ((result = read_file(path, &buffer)) == NO_ERR) {
    if (buffer.data) {
      zone_free(buffer.data);
      buffer.data = NULL;
      buffer.size = 0;
    }
  }
  return result;
}

