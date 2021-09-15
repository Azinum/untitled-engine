// diffuse.frag

#version 150

in vec2 uv_coord;
in vec3 surface_normal;
in vec3 frag_pos;

out vec4 out_color;

void main() {
	vec3 color = clamp(surface_normal, 0, 1);
	out_color = vec4(color, 1);
}
