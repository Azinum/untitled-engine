// mesh.c

#include "mesh_wavefront.c"

void mesh_init(Mesh* mesh) {
  mesh->data = NULL;

  mesh->vertex = NULL;
  mesh->vertex_count = 0;

  mesh->vertex_index = NULL;
  mesh->vertex_index_count = 0;

  mesh->uv = NULL;
  mesh->uv_count = 0;

  mesh->uv_index = NULL;
  mesh->uv_index_count = 0;

  mesh->normal = NULL;
  mesh->normal_count = 0;

  mesh->normal_index = NULL;
  mesh->normal_index_count = 0;
}

void mesh_print(FILE* fp, Mesh* mesh) {
  assert(fp);
  fprintf(fp,
    "mesh_print:\n"
    "  vertex_count: %u\n"
    "  vertex_index_count: %u\n"
    "  uv_count: %u\n"
    "  uv_index_count: %u\n"
    "  normal_count: %u\n"
    "  normal_index_count: %u\n"
    ,
    mesh->vertex_count,
    mesh->vertex_index_count,
    mesh->uv_count,
    mesh->uv_index_count,
    mesh->normal_count,
    mesh->normal_index_count
  );
}

i32 mesh_write(const char* path, Mesh* mesh) {
  i32 result = ERR;
  FILE* fp = fopen(path, "wb");
  if (!fp) {
    fprintf(stderr, "mesh_write: Failed to open file %s for writing\n", path);
    return ERR;
  }
  char* ext = get_extension(path);
  if (!string_cmp(ext, "obj")) {
    result = wavefront_mesh_write(fp, mesh);
  }
  else {
    fprintf(stderr, "mesh_write: Mesh format %s is not supported for file %s\n", ext, path);
  }
  fclose(fp);
  return result;
}

i32 mesh_load(const char* path, Mesh* mesh) {
  i32 result = ERR;
  char* ext = get_extension(path);
  mesh_init(mesh);
  Buffer mesh_data_buffer = {0};

  if ((result = read_file(path, &mesh_data_buffer)) != NO_ERR) {
    return result;
  }

  if (!string_cmp(ext, "obj")) {
    result = wavefront_mesh_load_from_buffer(&mesh_data_buffer, mesh);
  }
  else {
    fprintf(stderr, "mesh_load: Mesh format %s is not supported for file %s\n", ext, path);
  }
  buffer_free(&mesh_data_buffer);
  return result;
}

void mesh_unload(Mesh* mesh) {
  if (mesh->data) {
    zone_try_free(mesh->data);
  }
  else {
    zone_try_free(mesh->vertex);
    zone_try_free(mesh->vertex_index);
    zone_try_free(mesh->uv);
    zone_try_free(mesh->uv_index);
    zone_try_free(mesh->normal);
    zone_try_free(mesh->normal_index);
  }

  memory_set(mesh, 0, sizeof(Mesh));
}
