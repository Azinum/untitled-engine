// sprite.frag

#version 150

in vec2 uv;
out vec4 color;

uniform sampler2D tex;
uniform vec2 offset;
uniform vec2 range;

void main() {
	color = vec4(texture(tex, (uv * range) + offset).rgb, 1);
	if (color.r == 1 && color.g == 0 && color.b == 1) {
		discard;
	}
}
