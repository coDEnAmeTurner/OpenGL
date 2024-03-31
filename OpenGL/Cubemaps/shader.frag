#version 330

out vec4 fragcolor;

in vec3 normal;
in vec3 frag_pos;

uniform vec3 camera_pos;
uniform samplerCube sky_box;

void main() {
	float ratio = 1.00 / 1.52;
	vec3 I = normalize(frag_pos - camera_pos);
	vec3 R = refract(I, normalize(normal), ratio);

	fragcolor = vec4(texture(sky_box, R).rgb, 1.0);
}