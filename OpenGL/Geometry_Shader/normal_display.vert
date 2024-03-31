#version 330

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vert_tex_coords;

out VS_OUT {
	vec3 normal;
} vs_out;

uniform mat4 model, view, projection;

void main() {
	gl_Position = projection * view * model * vec4(vert_pos, 1);
	vs_out.normal = mat3(transpose(inverse(model))) * vert_normal;
}