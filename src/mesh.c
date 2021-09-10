// mesh.c

#include "mesh_wavefront.c"

static void mesh_sort_indices(Mesh* mesh);

void mesh_sort_indices(Mesh* mesh) {

}

void mesh_init(Mesh* mesh) {
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

i32 mesh_load(const char* path, Mesh* mesh) {
  char* ext = get_extension(path);
  if (!string_cmp(ext, "obj")) {
    return wavefront_mesh_load(path, mesh);
  }
  else {
    fprintf(stderr, "mesh_load: Mesh format %s not supported for file %s\n", ext, path);
  }
  return ERR;
}

void mesh_unload(Mesh* mesh) {

}
