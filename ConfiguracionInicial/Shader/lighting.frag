#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light1;  // Sol
uniform Light light2;  // Luna

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // === Luz 1 (sol) ===
    vec3 lightDir1 = normalize(light1.position - FragPos);
    float diff1 = max(dot(norm, lightDir1), 0.0);
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), material.shininess);
    vec3 result1 = light1.ambient * material.ambient
                 + light1.diffuse * diff1 * material.diffuse
                 + light1.specular * spec1 * material.specular;

    // === Luz 2 (luna) ===
    vec3 lightDir2 = normalize(light2.position - FragPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), material.shininess);
    vec3 result2 = light2.ambient * material.ambient
                 + light2.diffuse * diff2 * material.diffuse
                 + light2.specular * spec2 * material.specular;

    vec3 color = result1 + result2;
    FragColor = vec4(color, 1.0);
}
