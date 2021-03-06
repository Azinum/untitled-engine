// mesh.h

#ifndef _MESH_H
#define _MESH_H

typedef struct Mesh {
  u32  vertex_count;
  u32 vertex_index_count;
  u32 uv_count;
  u32 uv_index_count;
  u32 normal_count;
  u32 normal_index_count;

  void* data; // Data is stored contiguously if this is populated

  v3* vertex;
  u32* vertex_index;
  v2* uv;
  u32* uv_index;
  v3* normal;
  u32* normal_index;
} Mesh;

#include "mesh_wavefront.h"

void mesh_init(Mesh* mesh);

void mesh_print(FILE* fp, Mesh* mesh);

i32 mesh_write(const char* path, Mesh* mesh);

i32 mesh_write_out(const char* path, Mesh* mesh, FILE* fp);

i32 mesh_load(const char* path, Mesh* mesh);

void mesh_unload(Mesh* mesh);

#endif
