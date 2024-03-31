#version 330

layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec3 vert_normal;
layout (location = 2) in vec2 vert_tex_coords;

//out VS_OUT {
	//vec2 geom_tex_coords;
//} vs_out;

out vec2 frag_tex_coords;

uniform mat4 model, view, projection;

void main() {
	gl_Position = projection * view * model * vec4(vert_pos, 1);
	//vs_out.geom_tex_coords = vert_tex_coords;
	frag_tex_coords = vert_tex_coords;
}