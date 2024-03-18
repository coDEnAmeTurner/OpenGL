#version 330

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_tex;

uniform mat4 model, view, projection;

out vec2 frag_tex;

void main() {
	gl_Position = projection * view * model * vec4(vert_pos, 1.0f);
	frag_tex = vert_tex;

}