// diffuse.frag

#version 150

in vec2 uv_coord;
in vec3 surface_normal;
in vec3 frag_pos;

out vec4 out_color;

uniform sampler2D tex;

void main() {
	out_color = texture(tex, uv_coord);
}
