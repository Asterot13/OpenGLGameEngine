#version 330 core

struct Light
{
    vec3 position;
    vec3 color;
};

uniform Light uLight;

out vec4 FragColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

uniform sampler2D baseColorTexture;

void main()
{
    // Calculate diffuse lighting
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(uLight.position - vFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;    

    //Apply texture with diffuse lighting
	vec4 texColor = texture(baseColorTexture, vUV);
    FragColor = texColor * vec4(diffuse, 1.0);
}
