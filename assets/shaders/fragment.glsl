#version 330 core

struct Light
{
    vec3 position;
    vec3 color;
};

uniform Light uLight;
uniform vec3 uCameraPosition;

out vec4 FragColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

uniform sampler2D baseColorTexture;

void main()
{
	vec3 normal = normalize(vNormal);

    // Calculate diffuse lighting
    vec3 lightDir = normalize(uLight.position - vFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;

	// Calculate specular lighting
	vec3 viewDir = normalize(uCameraPosition - vFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float specularStrength = 0.5f;
	vec3 specular = specularStrength * spec * uLight.color;

	vec3 result = diffuse + specular;

    //Apply texture with lightning
	vec4 texColor = texture(baseColorTexture, vUV);
    FragColor = texColor * vec4(result, 1.0);
}
