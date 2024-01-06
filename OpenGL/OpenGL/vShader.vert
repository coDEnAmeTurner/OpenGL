#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	TexCoords = aTexCoords;

	Normal = mat3(transpose(inverse(model))) * aNormal;

	FragPos = vec3(model * vec4(aPos, 1.0f));

	gl_Position = projection * view * vec4(FragPos, 1.0f);
}