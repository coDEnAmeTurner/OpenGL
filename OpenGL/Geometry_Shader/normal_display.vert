#version 330 core

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_normal;

out VS_OUT {
	vec3 normal;
} vs_out;

uniform mat4 model, view;

void main() {
	gl_Position = view * model * vec4(vert_pos, 1);
	vs_out.normal = normalize((mat3(transpose(inverse(model))) * vert_normal));
}