#version 330

out vec4 fragcolor;

in vec2 fragtex;

uniform sampler2D colortex;

void main() {
	fragcolor = texture(colortex, fragtex);
}