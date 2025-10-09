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
in vec2 TexCoords;

out vec4 color;

uniform vec3 viewPos;
uniform Material material;
uniform Light light1;
uniform Light light2;

uniform sampler2D texture_diffusse;

// Función auxiliar para calcular la contribución de una luz
vec3 CalcLight(Light light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
    // Dirección de la luz
    vec3 lightDir = normalize(light.position - fragPos);

    // --- Difusa ---
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * material.diffuse;

    // --- Especular ---
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);

    // --- Ambiental ---
    vec3 ambient = light.ambient * material.ambient;

    return (ambient + diffuse + specular);
}

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Cálculo de iluminación de ambas luces
    vec3 result1 = CalcLight(light1, norm, FragPos, viewDir);
    vec3 result2 = CalcLight(light2, norm, FragPos, viewDir);

    // Suma de ambas luces
    vec3 lighting = result1 + result2;

    // Combinar con textura
    vec3 texColor = texture(texture_diffusse, TexCoords).rgb;
    vec3 finalColor = lighting * texColor;

    color = vec4(finalColor, 1.0);
}
