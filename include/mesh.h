// mesh.h

#ifndef _MESH_H
#define _MESH_H

typedef struct Mesh {
  v3* vertex;
  u32  vertex_count;

  u32* vertex_index;
  u32 vertex_index_count;

  v2* uv;
  u32 uv_count;

  v3* normal;
  u32 normal_count;
} Mesh;

void mesh_init(Mesh* mesh);

i32 mesh_load(const char* path, Mesh* mesh);

void mesh_unload(Mesh* mesh);

#endif
