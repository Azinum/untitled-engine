// rect.vert

#version 150

in vec4 vertex;
out vec2 uv;

uniform mat4 proj;
uniform mat4 model;

void main() {
	uv = vertex.zw;
	gl_Position = proj * model * vec4(vertex.xy, 0, 1);
}
