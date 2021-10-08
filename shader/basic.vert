// sprite.vert

#version 150

in vec4 vertex;
out vec2 uv;

uniform mat4 projection;
uniform mat4 model;

void main() {
	uv = vertex.zw;
	gl_Position = projection * model * vec4(vertex.xy, 0, 1);
}
