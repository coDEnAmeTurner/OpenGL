#version 330

in vec2 frag_tex;

uniform sampler2D texture;

out vec4 fragcolor;

void main() {
	fragcolor = texture(texture, frag_tex);
	//fragcolor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}