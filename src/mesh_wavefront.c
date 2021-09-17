// mesh_wavefront.c

#define MAX_LINE_SIZE 256

#define safe_scanf(ScanStatus, Iterator, Format, ...) { \
  i32 num_bytes_read = 0; \
  ScanStatus = sscanf(Iterator, Format "%n", __VA_ARGS__, &num_bytes_read); \
  Iterator += num_bytes_read; \
}

// Prefetch mesh information (number of vertices, normals, indices e.t.c.), and load that information into the mesh itself
static i32 wavefront_prepare_mesh(Buffer* buffer, Mesh* mesh, const u8 sort);
static i32 wavefront_parse_mesh(Buffer* buffer, Mesh* mesh);
static i32 wavefront_sort_mesh(Mesh* mesh);

i32 wavefront_prepare_mesh(Buffer* buffer, Mesh* mesh, const u8 sort) {
  i32 result = NO_ERR;
  i32 scan_status = 0;
  char line[MAX_LINE_SIZE] = {0};
  char* iter = &buffer->data[0];

  do {
    safe_scanf(scan_status, iter, "%s\n", line);
    if (scan_status == EOF) {
      break;
    }

    if (!string_n_cmp(line, "v", MAX_LINE_SIZE)) {
      mesh->vertex_count++;
    }
    else if (!string_n_cmp(line, "vt", MAX_LINE_SIZE)) {
      mesh->uv_count++;
    }
    else if (!string_n_cmp(line, "vn", MAX_LINE_SIZE)) {
      mesh->normal_count++;
    }
    else if (!string_n_cmp(line, "f", MAX_LINE_SIZE)) {
      mesh->vertex_index_count += 3;
      mesh->uv_index_count += 3;
      mesh->normal_index_count += 3;
    }
  } while (1);

  if (sort) {
    mesh->uv_count = mesh->uv_index_count;
    mesh->normal_count = mesh->normal_index_count;
  }

#define LINEAR_STORAGE 1

#if LINEAR_STORAGE
  const u32 size = sizeof(v3) * mesh->vertex_count +
    sizeof(u32) * mesh->vertex_index_count +
    sizeof(v2) * mesh->uv_count +
    sizeof(u32) * mesh->uv_index_count +
    sizeof(v3) * mesh->normal_count +
    sizeof(u32) * mesh->normal_index_count;
  mesh->data = zone_malloc(size);

  mesh->vertex = (void*)mesh->data;
  mesh->vertex_index = (void*)((u8*)mesh->vertex + (sizeof(v3) * mesh->vertex_count));

  mesh->uv = (void*)((u8*)mesh->vertex_index + (sizeof(u32) * mesh->vertex_index_count));
  mesh->uv_index = (void*)((u8*)mesh->uv + (sizeof(v2) * mesh->uv_count));

  mesh->normal = (void*)((u8*)mesh->uv_index + (sizeof(u32) * mesh->uv_index_count));
  mesh->normal_index = (void*)((u8*)mesh->normal + (sizeof(v3) * mesh->normal_count));
#else
  mesh->vertex = zone_malloc(sizeof(v3) * mesh->vertex_count);
  mesh->vertex_index = zone_malloc(sizeof(u32) * mesh->vertex_index_count);

  mesh->uv = zone_malloc(sizeof(v2) * mesh->uv_count);
  mesh->uv_index = zone_malloc(sizeof(u32) * mesh->uv_index_count);

  mesh->normal = zone_malloc(sizeof(v3) * mesh->normal_count);
  mesh->normal_index = zone_malloc(sizeof(u32) * mesh->normal_index_count);
#endif
  return result;
}

i32 wavefront_parse_mesh(Buffer* buffer, Mesh* mesh) {
  i32 result = NO_ERR;
  i32 scan_status = 0;
  char line[MAX_LINE_SIZE] = {0};
  char* iter = &buffer->data[0];

  v3* vertex = &mesh->vertex[0];
  u32* vertex_index = &mesh->vertex_index[0];
  v2* uv = &mesh->uv[0];
  u32* uv_index = &mesh->uv_index[0];
  v3* normal = &mesh->normal[0];
  u32* normal_index = &mesh->normal_index[0];

  do {
    safe_scanf(scan_status, iter, "%s\n", line);
    if (scan_status == EOF) {
      break;
    }

    if (!string_n_cmp(line, "v", MAX_LINE_SIZE)) {
      v3 v;
      safe_scanf(scan_status, iter, "%f %f %f", &v.x, &v.y, &v.z);
      *vertex++ = v;
    }
    else if (!string_n_cmp(line, "vt", MAX_LINE_SIZE)) {
      v2 v;
      safe_scanf(scan_status, iter, "%f %f", &v.x, &v.y);
      *uv++ = v;
    }
    else if (!string_n_cmp(line, "vn", MAX_LINE_SIZE)) {
      v3 v;
      safe_scanf(scan_status, iter, "%f %f %f", &v.x, &v.y, &v.z);
      *normal++ = v;
    }
    else if (!string_n_cmp(line, "f", MAX_LINE_SIZE)) {
      u32 vi[3] = {0};  // Vertex indices
      u32 ui[3] = {0};  // UV indices
      u32 ni[3] = {0};  // Normal indices
      safe_scanf(scan_status, iter,
        "%u/%u/%u %u/%u/%u %u/%u/%u\n",
        &vi[0], &ui[0], &ni[0],
        &vi[1], &ui[1], &ni[1],
        &vi[2], &ui[2], &ni[2]
      );
      if (scan_status != 9) {
        fprintf(stderr, "wavefront_parse_mesh: Failed to parse object file\n");
        result = ERR;
        goto done;
      }
      *vertex_index++ = vi[0] - 1;
      *vertex_index++ = vi[1] - 1;
      *vertex_index++ = vi[2] - 1;

      *uv_index++ = ui[0] - 1;
      *uv_index++ = ui[1] - 1;
      *uv_index++ = ui[2] - 1;

      *normal_index++ = ni[0] - 1;
      *normal_index++ = ni[1] - 1;
      *normal_index++ = ni[2] - 1;
    }
  } while (1);
done:
  return result;
}

i32 wavefront_sort_mesh(Mesh* mesh) {
  i32 result = NO_ERR;

  u32 uv_count = mesh->uv_count;
  v2* uv = zone_malloc(uv_count * sizeof(v2));;
  u32 normal_count = mesh->normal_count;
  v3* normal = zone_malloc(normal_count * sizeof(v3));
  if (uv && normal) {
    memory_copy(uv, mesh->uv, uv_count * sizeof(v2));
    memory_copy(normal, mesh->normal, normal_count * sizeof(v3));

    for (u32 i = 0; i < mesh->vertex_index_count; ++i) {
      u32 index = mesh->vertex_index[i];
      u32 uv_index = mesh->uv_index[i];
      u32 normal_index = mesh->normal_index[i];

      mesh->uv[index] = uv[uv_index];
      mesh->normal[index] = normal[normal_index];
    }

    zone_free(uv);
    zone_free(normal);
  }
  else {
    result = ERR;
  }
  return result;
}

i32 wavefront_mesh_write(FILE* fp, Mesh* mesh) {
  assert(fp);

  for (u32 i = 0; i < mesh->vertex_count; ++i) {
    v3 v = mesh->vertex[i];
    fprintf(fp, "v %f %f %f\n", v.x, v.y, v.z);
  }
  for (u32 i = 0; i < mesh->uv_count; ++i) {
    v2 v = mesh->uv[i];
    fprintf(fp, "vt %f %f\n", v.x, v.y);
  }
  for (u32 i = 0; i < mesh->normal_count; ++i) {
    v3 v = mesh->normal[i];
    fprintf(fp, "vn %f %f %f\n", v.x, v.y, v.z);
  }
  for (u32 i = 0; i < mesh->vertex_index_count; i += 3) {
      fprintf(fp,
        "f %u/%u/%u %u/%u/%u %u/%u/%u\n",
        mesh->vertex_index[i + 0] + 1, mesh->uv_index[i + 0] + 1, mesh->normal_index[i + 0] + 1,
        mesh->vertex_index[i + 1] + 1, mesh->uv_index[i + 1] + 1, mesh->normal_index[i + 1] + 1,
        mesh->vertex_index[i + 2] + 1, mesh->uv_index[i + 2] + 1, mesh->normal_index[i + 2] + 1
      );
  }
  return NO_ERR;
}

i32 wavefront_mesh_load_from_buffer(Buffer* buffer, Mesh* mesh) {
  i32 result = NO_ERR;
#define SORT 1

  if ((result = wavefront_prepare_mesh(buffer, mesh, SORT)) == NO_ERR) {
    if ((result = wavefront_parse_mesh(buffer, mesh)) == NO_ERR) {
#if SORT
      if ((result = wavefront_sort_mesh(mesh)) != NO_ERR) {
        fprintf(stderr, "wavefront_mesh_load_from_buffer: Failed to sort mesh data\n");
      }
#endif
    }
    else {
      fprintf(stderr, "wavefront_mesh_load_from_buffer: Failed to parse mesh data\n");
    }
  }
  else {
    fprintf(stderr, "wavefront_mesh_load_from_buffer: Failed to prepare mesh\n");
  }
  return result;
}
