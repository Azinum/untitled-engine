// diffuse2.frag

#version 150

in vec2 uv_coord;
in vec3 surface_normal;
in vec3 frag_pos;

out vec4 out_color;

uniform sampler2D tex;

vec3 light_pos = vec3(4.0, 0.0, 4.0);
vec3 light_color = vec3(1.0, 1.0, 1.0) * 1.2;

void main() {
	float ambient_strength = 0.1;
	vec3 ambient = ambient_strength * light_color;

	vec3 normal = normalize(surface_normal);
	vec3 light_direction = normalize(light_pos - frag_pos);
	float delta = max(dot(normal, light_direction), 0.0);
	vec3 diffuse = delta * light_color;

	out_color = vec4((ambient + diffuse), 1.0) * texture(tex, uv_coord);
}
