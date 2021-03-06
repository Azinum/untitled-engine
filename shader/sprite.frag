// sprite.frag

#version 150

in vec2 uv;
out vec4 color;

uniform sampler2D tex;

void main() {
	color = vec4(texture(tex, uv).rgb, 1);
	if (color.r == 1 && color.g == 0 && color.b == 1) {
		discard;
	}
}
