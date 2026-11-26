#version 330 core

struct Light
{
    vec3 direction;
    vec3 color;
};

uniform Light uLight;
uniform vec3 uCameraPosition;
uniform vec3 color = vec3(1.0);

out vec4 FragColor;

in vec2 vUV;
in vec3 vNormal;
in vec3 vFragPos;

uniform sampler2D baseColorTexture;

void main()
{
	vec3 normal = normalize(vNormal);

    // Calculate diffuse lighting
    vec3 lightDir = normalize(-uLight.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * uLight.color;

	// Calculate specular lighting
	vec3 viewDir = normalize(uCameraPosition - vFragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	float specularStrength = 0.5f;
	vec3 specular = specularStrength * spec * uLight.color;

    // Calculating ambient lighting
    const float ambientStrength = 0.4f;
    vec3 ambient = ambientStrength * uLight.color;

	vec3 resLight = diffuse + specular + ambient;

    //Apply texture with lightning, final color calculation
	vec4 texColor = texture(baseColorTexture, vUV);
    vec3 resColor = resLight * texColor.xyz * color;
    FragColor = vec4(resColor, 1.0);
}
