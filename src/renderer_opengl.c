// renderer_opengl.c

#include <GL/glew.h>

#if __APPLE__
  #include <OpenGL/gl.h>
  #include <OpenGL/glu.h>
#else
  #include <GL/gl.h>
#endif

#include "shader.c"

u32 quad_vao = 0,
  quad_vbo = 0;
u32 basic_shader = 0;
Image sprite_source;
u32 sprite = 0;

static i32 opengl_init();
static i32 shader_compile_from_source(const char* vert_source, const char* frag_source, u32* program_out);
static void upload_vertex_data(f32* data, u32 size, u32 attr_size, u32 attr_count, u32* restrict vao, u32* restrict vbo);
static void upload_texture(Image* texture, u32* texture_id);

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

#define SHADER_ERROR_BUFFER_SIZE 512

i32 shader_compile_from_source(const char* vert_source, const char* frag_source, u32* program_out) {
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
	glLinkProgram(program);

	glGetProgramiv(program, GL_VALIDATE_STATUS, &compile_report);
#if 1
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

void render_rect(v3 position, v3 size) {
  u32 handle = basic_shader;
  glUseProgram(handle);

  model = translate(position);
  model = m4_multiply(model, scale(size));

  glUniformMatrix4fv(glGetUniformLocation(handle, "proj"), 1, GL_FALSE, (float*)&orthogonal_proj);
  glUniformMatrix4fv(glGetUniformLocation(handle, "model"), 1, GL_FALSE, (float*)&model);

  v2 offset = V2(0.0f, 0);
  v2 range = V2(0.5f, 1.0f);

  glUniform2f(glGetUniformLocation(handle, "offset"), offset.x, offset.y);
  glUniform2f(glGetUniformLocation(handle, "range"), range.x, range.y);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, sprite);

  glBindVertexArray(quad_vao);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindVertexArray(0);

  glUseProgram(0);
}

i32 renderer_init() {
  opengl_init();
  upload_vertex_data(quad_vertices, sizeof(quad_vertices), sizeof(float) * 4, 4, &quad_vao, &quad_vbo);
  shader_compile_from_source(rect_vert, rect_frag, &basic_shader);
  image_load("resource/sprite/spritesheet.bmp", &sprite_source);
  upload_texture(&sprite_source, &sprite);
  orthogonal_proj = orthographic(0, 800.0f, 600.0f, 0, -1.0f, 1.0f);
  return NO_ERR;
}

void renderer_clear(u8 r, u8 g, u8 b) {
  glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderer_free() {
  image_unload(&sprite_source);
  glDeleteVertexArrays(1, &quad_vao);
  glDeleteVertexArrays(1, &quad_vbo);
  glDeleteProgram(basic_shader);
}
