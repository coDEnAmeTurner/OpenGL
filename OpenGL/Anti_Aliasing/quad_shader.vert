#version 330

layout (location = 0) in vec2 vert_pos;
layout (location = 1) in vec2 vert_tex;

out vec2 fragtex;

void main() {
	fragtex = vert_tex;
	gl_Position = vec4(vert_pos.x, vert_pos.y, 0.0f, 1.0f);

}