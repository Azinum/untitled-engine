// renderer_opengl.c

#include <GL/glew.h>

#if __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
#endif

#include "shader.c"

#define DRAW_CALL(call) call; renderer.draw_calls++

u32 quad_vao = 0,
  quad_vbo = 0;
u32 basic_shader = 0,
  diffuse_shader = 0,
  diffuse2_shader = 0;

typedef struct Model {
  u32 draw_count;
  u32 vao;
  u32 vbo;
  u32 ebo;
} Model;

#define MAX_MODEL 32
#define MAX_TEXTURE 32

typedef struct Renderer {
  u32 draw_calls;

  Model models[MAX_MODEL];
  u32 model_count;

  u32 textures[MAX_TEXTURE];
  u32 texture_count;
} Renderer;

Renderer renderer;

static i32 opengl_init();
static i32 init_state(Renderer* r);
static i32 shader_compile_from_source(const char* vert_source, const char* frag_source, const char* attrib_locations, u32* program_out);
static void upload_vertex_data(f32* data, u32 size, u32 attr_size, u32 attr_count, u32* restrict vao, u32* restrict vbo);
static void upload_texture(Image* texture, u32* texture_id);
static void upload_model(Mesh* mesh, Model* model);
static void store_attribute(Model* model, i32 attribute_index, u32 count, u32 size, void* data);

i32 opengl_init() {
  i32 glew_err = glewInit();
  if (glew_err != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(glew_err));
    return ERR;
  }

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glShadeModel(GL_FLAT);
  glEnable(GL_BLEND);
  glEnable(GL_TEXTURE_2D);
  glAlphaFunc(GL_GREATER, 1);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glEnable(GL_TEXTURE_CUBE_MAP_EXT);
  glEnable(GL_TEXTURE_GEN_S);
  glEnable(GL_TEXTURE_GEN_R);
  glEnable(GL_TEXTURE_GEN_T);

  fprintf(stdout, "GL VENDOR:    %s\n", glGetString(GL_VENDOR));
  fprintf(stdout, "GL RENDERER:  %s\n", glGetString(GL_RENDERER));
  fprintf(stdout, "GL VERSION:   %s\n", glGetString(GL_VERSION));
  fprintf(stdout, "GLSL VERSION: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

  return NO_ERR;
}

i32 init_state(Renderer* r) {
  r->draw_calls = 0;
  r->model_count = 0;
  r->texture_count = 0;
  return NO_ERR;
}

#define SHADER_ERROR_BUFFER_SIZE 512

i32 shader_compile_from_source(const char* vert_source, const char* frag_source, const char* attrib_locations, u32* program_out) {
  i32 result = NO_ERR;
  i32 compile_report = 0;
  u32 program = 0;
  char err_log[SHADER_ERROR_BUFFER_SIZE] = {0};
  u32 vert_shader = 0, frag_shader = 0;

  vert_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vert_shader, 1, &vert_source, NULL);
  glCompileShader(vert_shader);

  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &compile_report);
  if (!compile_report) {
    glGetShaderInfoLog(vert_shader, SHADER_ERROR_BUFFER_SIZE, NULL, err_log);
    fprintf(stderr, "error in vertex shader: %s\n", err_log);
    result = ERR;
    goto done;
  }

  frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(frag_shader, 1, &frag_source, NULL);
  glCompileShader(frag_shader);

  glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &compile_report);
  if (!compile_report) {
    glGetShaderInfoLog(frag_shader, SHADER_ERROR_BUFFER_SIZE, NULL, err_log);
    fprintf(stderr, "error in fragment shader: %s\n", err_log);
    result = ERR;
    goto done;
  }

  program = glCreateProgram();
  glAttachShader(program, vert_shader);
  glAttachShader(program, frag_shader);

  // Bind attribute locations before linking
  if (attrib_locations) {
    char line[MAX_LINE_SIZE] = {0};
    char* iter = (char*)&attrib_locations[0];
    do {
      i32 scan_status = 0;
      safe_scanf(scan_status, iter, "%s\n", line);
      if (scan_status == EOF) {
        break;
      }

      if (!string_n_cmp(line, "attribute", MAX_LINE_SIZE)) {
        char name[MAX_LINE_SIZE] = {0};
        i32 location = -1;
        safe_scanf(scan_status, iter, "%s", name);
        safe_scanf(scan_status, iter, "%i", &location);
        if (location >= 0) {
          glBindAttribLocation(program, location, name);
        }
      }
    } while (1);
  }

  glLinkProgram(program);

  glGetProgramiv(program, GL_VALIDATE_STATUS, &compile_report);
#if 0
  if (!compile_report) {
    glGetProgramInfoLog(program, SHADER_ERROR_BUFFER_SIZE, NULL, err_log);
    fprintf(stderr, "shader compile error: %s\n", err_log);
    goto done;
  }
#endif
  *program_out = program;
done:
  if (vert_shader > 0)
    glDeleteShader(vert_shader);
  if (frag_shader > 0)
    glDeleteShader(frag_shader);
  return result;
}

void upload_vertex_data(f32* data, u32 size, u32 attr_size, u32 attr_count, u32* restrict vao, u32* restrict vbo) {
  glGenVertexArrays(1, vao);
  glGenBuffers(1, vbo);

  glBindBuffer(GL_ARRAY_BUFFER, *vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

  glBindVertexArray(*vao);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(0, attr_count, GL_FLOAT, GL_FALSE, attr_size, (void*)0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
}

void upload_texture(Image* texture, u32* texture_id) {
  glGenTextures(1, texture_id);
  glBindTexture(GL_TEXTURE_2D, *texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void upload_model(Mesh* mesh, Model* model) {
  model->draw_count = mesh->vertex_index_count;

  glGenVertexArrays(1, &model->vao);
  glBindVertexArray(model->vao);

  glGenBuffers(1, &model->ebo);

  store_attribute(model, 0, 3, mesh->vertex_count * sizeof(v3), &mesh->vertex[0]);
  store_attribute(model, 1, 2, mesh->uv_count * sizeof(v2), &mesh->uv[0]);
  store_attribute(model, 2, 3, mesh->normal_count * sizeof(v3), &mesh->normal[0]);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->vertex_index_count * sizeof(u32), &mesh->vertex_index[0], GL_STATIC_DRAW);

  glBindVertexArray(0);
}

void store_attribute(Model* model, i32 attribute_index, u32 count, u32 size, void* data) {
  glEnableVertexAttribArray(attribute_index);
  glGenBuffers(1, &model->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, model->vbo);
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(attribute_index, count, GL_FLOAT, GL_FALSE, 0, NULL);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render_texture(i32 texture_id, v3 position, v3 size) {
  if (texture_id < 0 || texture_id >= renderer.texture_count) return;

  u32 handle = basic_shader;
  glUseProgram(handle);

  u32 texture = renderer.textures[texture_id];

  model = translate(position);
  model = m4_multiply(model, scale(size));

  glUniformMatrix4fv(glGetUniformLocation(handle, "projection"), 1, GL_FALSE, (f32*)&orthogonal_proj);
  glUniformMatrix4fv(glGetUniformLocation(handle, "model"), 1, GL_FALSE, (f32*)&model);

#if 0
  v2 offset = V2((1.0f / 20.0f) * 1, 0.0f);
  v2 range = V2((1.0f / 20.0f), 1.0f);
#else
  v2 offset = V2(0, 0);
  v2 range = V2(1, 1);
#endif
  glUniform2f(glGetUniformLocation(handle, "offset"), offset.x, offset.y);
  glUniform2f(glGetUniformLocation(handle, "range"), range.x, range.y);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  glBindVertexArray(quad_vao);
  DRAW_CALL(glDrawArrays(GL_TRIANGLES, 0, 6));
  glBindVertexArray(0);

  glUseProgram(0);
}

void render_model(i32 model_id, i32 texture_id, v3 position, v3 size) {
  if (model_id < 0 || model_id >= renderer.model_count) return;
  if (texture_id < 0 || texture_id >= renderer.texture_count) return;

  u32 handle = diffuse2_shader;
  glUseProgram(handle);

  u32 texture = renderer.textures[texture_id];

  model = translate(position);

  // TODO(lucas): Rotations

  model = m4_multiply(model, scale(size));

  glUniformMatrix4fv(glGetUniformLocation(handle, "projection"), 1, GL_FALSE, (f32*)&perspective_proj);
  glUniformMatrix4fv(glGetUniformLocation(handle, "view"), 1, GL_FALSE, (f32*)&view);
  glUniformMatrix4fv(glGetUniformLocation(handle, "model"), 1, GL_FALSE, (f32*)&model);

  Model* m = &renderer.models[model_id];

  glBindVertexArray(m->vao);
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
  glEnableVertexAttribArray(2);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  DRAW_CALL(glDrawElements(GL_TRIANGLES, m->draw_count, GL_UNSIGNED_INT, 0));

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(2);

  glBindVertexArray(0);

  glUseProgram(0);
}

i32 renderer_upload_mesh(Mesh* mesh) {
  assert(mesh);
  i32 id = -1;
  if (renderer.model_count < MAX_MODEL) {
    id = renderer.model_count;
    Model* model = &renderer.models[renderer.model_count++];
    upload_model(mesh, model);
  }
  return id;
}

i32 renderer_upload_texture(Image* texture) {
  assert(texture);
  i32 id = -1;
  if (renderer.texture_count < MAX_TEXTURE) {
    id = renderer.texture_count;
    u32* texture_id = &renderer.textures[renderer.texture_count++];
    upload_texture(texture, texture_id);
  }
  return id;
}

i32 renderer_init() {
  opengl_init();
  init_state(&renderer);

  view = m4d(1.0f);
  renderer_framebuffer_cb(platform_window_width(), platform_window_height());

  shader_compile_from_source(sprite_vert, sprite_frag, NULL, &basic_shader);
  shader_compile_from_source(diffuse_vert, diffuse_frag, diffuse_attribs, &diffuse_shader);
  shader_compile_from_source(diffuse2_vert, diffuse2_frag, diffuse2_attribs, &diffuse2_shader);

  upload_vertex_data(quad_vertices, sizeof(quad_vertices), sizeof(float) * 4, 4, &quad_vao, &quad_vbo);
  return NO_ERR;
}

void renderer_begin_frame() {
  renderer.draw_calls = 0;
}

void renderer_end_frame(u8 r, u8 g, u8 b) {
  platform_swap_buffers();
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_free() {
  glDeleteVertexArrays(1, &quad_vao);
  glDeleteVertexArrays(1, &quad_vbo);
  glDeleteProgram(basic_shader);
  glDeleteProgram(diffuse_shader);
  glDeleteProgram(diffuse2_shader);

  for (u32 i = 0; i < renderer.model_count; ++i) {
    Model* model = &renderer.models[i];
    glDeleteVertexArrays(1, &model->vao);
    glDeleteVertexArrays(1, &model->vbo);
    glDeleteBuffers(1, &model->ebo);
  }
  for (u32 i = 0; i < renderer.texture_count; ++i) {
    u32* texture = &renderer.textures[i];
    glDeleteTextures(1, texture);
  }
}
