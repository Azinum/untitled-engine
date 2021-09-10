// mesh.c

static i32 wavefront_mesh_load(const char* path, Mesh* mesh);
static void mesh_sort_indices(Mesh* mesh);

i32 wavefront_mesh_load(const char* path, Mesh* mesh) {
  return NO_ERR;
}

void mesh_sort_indices(Mesh* mesh) {

}

void mesh_init(Mesh* mesh) {
  mesh->vertex = NULL;
  mesh->vertex_count = 0;

  mesh->vertex_index = NULL;
  mesh->vertex_index_count = 0;

  mesh->uv = NULL;
  mesh->uv_count = 0;

  mesh->normal = NULL;
  mesh->normal_count = 0;
}

i32 mesh_load(const char* path, Mesh* mesh) {
  char* ext = get_extension(path);
  if (!string_cmp(ext, "obj")) {
    return wavefront_mesh_load(path, mesh);
  }
  else {
    fprintf(stderr, "Mesh format %s not supported for file %s\n", ext, path);
  }
  return ERR;
}

void mesh_unload(Mesh* mesh) {

}
