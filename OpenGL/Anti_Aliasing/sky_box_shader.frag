#version 330

in vec3 frag_tex;

uniform samplerCube texture;

out vec4 fragcolor;

void main() {
	fragcolor = texture(texture, frag_tex);
}