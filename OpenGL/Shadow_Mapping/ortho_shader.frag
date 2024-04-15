#version 330 core

out vec4 fragcolor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D wood;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec4 lightColor;

uniform vec3 camPos;

float ShadowCalculation(vec4 fragPosLightSpace, float bias) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
	    for(int y = -1; y <= 1; ++y)
        {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;

    if (projCoords.z > 1.0)
        shadow = 0.0;

    return shadow;
}

const float ambientStrength = 0.1;

void main() {
    vec4 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(fs_in.Normal);
    vec3 lDir = normalize(lightPos - fs_in.FragPos);
    float diff = max(dot(norm, lDir), 0.0);
    vec4 diffuse = diff * lightColor;
    
    vec3 reflectDir = normalize(reflect(-lDir, norm));
    vec3 view = normalize(fs_in.FragPos - camPos);
    float shininess = 32;
    vec4 specular = pow(max(dot(view, reflectDir), 0.0), shininess) * lightColor;

    float bias = max(0.05 * (1.0 - dot(norm, lDir)), 0.005);
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, bias);
    
    vec4 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * texture(wood, fs_in.TexCoords);

    fragcolor = result;
}