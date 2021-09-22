// diffuse2.vert

#version 150

in vec3 position;
in vec2 uv;
in vec3 normal;

out vec2 uv_coord;
out vec3 surface_normal;
out vec3 frag_pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
	uv_coord = vec2(uv.x, 1 - uv.y);
	surface_normal = normal;
	frag_pos = vec3(model * vec4(position, 1));
	gl_Position = projection * view * model * vec4(position, 1);
}
