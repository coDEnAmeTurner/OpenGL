#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D Image;
uniform float near;
uniform float far;

float LinearizeDepth(float depth)
{
	float z = depth * 2.0 - 1.0; // back to NDC
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {

	FragColor = texture(Image, TexCoords);

}