#version 330

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_normal;

out vec3 normal;
out vec3 frag_pos;

uniform mat4 model, view, projection;

void main() {
	normal = mat3(transpose(inverse(model))) * vert_normal;
	frag_pos = vec3(model * vec4(vert_pos, 1.0f));
	gl_Position = projection * view * vec4(frag_pos, 1.0f);

}