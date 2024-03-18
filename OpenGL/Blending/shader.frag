#version 330

in vec2 frag_tex;

uniform sampler2D texture;

out vec4 fragcolor;

void main() {
	fragcolor = texture(texture, frag_tex);
	if (fragcolor.a < 0.75)
		discard;
}