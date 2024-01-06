#version 330 core

struct Material {
	sampler2D texture_diffuse0; //might be changed to 0
	sampler2D texture_specular0;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;

	float constant;
	float linear;
	float quadratic;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;
uniform PointLight lights[2];
uniform vec3 cameraFront;

out vec4 FragColor;

float calculatePointLight_Attenuation(PointLight light) {
	float distance    = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	return attenuation;
}

vec4 calculatePointLight_AmbientDiffuse(PointLight light) {
	vec3 ambient = light.ambient;

	float diffuseFactor = max(dot(normalize(Normal), normalize(light.position - FragPos)), 0.0f);

	vec3 diffuse = light.diffuse * diffuseFactor;

	if (diffuseFactor > 0)
		diffuse *= calculatePointLight_Attenuation(light);

	vec4 amDifResult = vec4(ambient + diffuse, 1.0f) * texture(material.texture_diffuse0, TexCoords);

	return amDifResult;
}

vec4 calculatePointLight_Specular(PointLight light) {
	vec3 reflectedLight =  reflect(normalize(FragPos - light.position), normalize(Normal));
	float specularIntensity = max(dot(normalize(reflectedLight), -normalize(cameraFront)), 0.0f);
	float specularFactor = pow(specularIntensity, light.shininess);

	vec3 specular = light.specular * specularFactor; 

	vec4 specResult = vec4(specular, 1.0f) * texture(material.texture_specular0, TexCoords);

	if (specularIntensity > 0) 
		specResult *= calculatePointLight_Attenuation(light);

	return specResult;
}


void main() {

	vec4 result = vec4(0.0f);

	for (int i = 0; i < 2; i++) {
		result += calculatePointLight_AmbientDiffuse(lights[i])
				+ calculatePointLight_Specular(lights[i]);
	}

	FragColor = result;
}