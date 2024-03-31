#version 330

out vec4 fragcolor;

in vec2 frag_tex_coords;

struct Material {
	sampler2D texture_diffuse1;

};

uniform Material material;

void main() {
	fragcolor = texture(material.texture_diffuse1, frag_tex_coords);
}