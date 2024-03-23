#version 330

in vec2 frag_tex;

uniform sampler2D texture;

out vec4 fragcolor;

const float offset = 1/300.0;

void main() {
	vec2 offsets[9] = vec2[] (
		vec2(-offset, offset), //top left
		vec2(0.0f, offset), //top center
		vec2(offset, offset), //top right
		vec2(-offset, 0.0f), //center left
		vec2(0.0f, 0.0f), //center center
		vec2(offset, 0.0f), //center right
		vec2(-offset, -offset), //bottom left
		vec2(0, -offset), //bottom center,
		vec2(offset, -offset) //bottom right
	);

	float kernel[9] = {
		1, 1, 1,
		1, -8, 1, 
		1, 1, 1
	};

	vec3 sampleTex[9];
	for (int i = 0; i < 9; i++) {
		sampleTex[i] = vec3(texture(texture, frag_tex.st + offsets[i]));
	}
	vec3 color = vec3(0.0);
	for (int i = 0; i < 9; i++) {
		color += sampleTex[i] * kernel[i];
	}
	fragcolor = vec4(color, 1.0);
}