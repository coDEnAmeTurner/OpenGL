#version 330

layout (location = 0) in vec3 vert_pos;

uniform mat4 view, projection;

out vec3 frag_tex;

void main() {
	vec4 transformed_pos = projection * view * vec4(vert_pos, 1.0f);
	gl_Position = transformed_pos.xyww;
	frag_tex = vert_pos;

}