#version 330 core
out vec4 FragColor;

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;

    float cutOffAngle;
    float outerCutOff;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    // sampler2D emission1;
    // float shininess;   

};

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, bool hasTex);
vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, bool hasTex);

uniform SpotLight spotLight;
uniform DirLight dirLight;
uniform Material material;

in vec2 TexCoord;
in vec3 normal;
in vec3 FragPos;
in vec4 color;
in float useTexture;

uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec4 modelColor;

void main()
{    
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = vec3(0.0);

    bool hasTex = useTexture > 0.5;

    // result += calcSpotLight(spotLight, norm, FragPos, viewDir, hasTex);
    result += calcDirLight(dirLight, norm, viewDir, hasTex);

    if (hasTex) {
        FragColor = vec4(result, 1.0) * modelColor;
    } else {
        // Si no hay textura, mezclamos la iluminación con el color del vértice
        FragColor = vec4(result * color.rgb, 1.0) * modelColor;
    }
    
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, bool hasTex) {
    // 1. Obtener color base (difuso)
    vec3 baseColor = hasTex ? texture(material.texture_diffuse1, TexCoord).rgb : vec3(1.0);
    
    // 2. Obtener color especular
    vec3 specColor = hasTex ? texture(material.texture_specular1, TexCoord).rgb : vec3(0.5);

     // ambient
    vec3 ambient = light.ambient * texture(material.texture_specular1, TexCoord).rgb;

    // diffuse 
   
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * baseColor;

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
    
    // specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.specular * spec * specColor;  


    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOffAngle - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;

    return diffuse + specular + ambient;
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir, bool hasTex) {
    // 1. Obtener color base (difuso)
    vec3 baseColor = hasTex ? texture(material.texture_diffuse1, TexCoord).rgb : vec3(1.0);
    
    // 2. Obtener color especular
    vec3 specColor = hasTex ? texture(material.texture_specular1, TexCoord).rgb : vec3(0.5);

    // Ambient
    vec3 ambient = light.ambient * baseColor;

    // Diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * baseColor;

    // Specular
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = light.specular * spec * specColor; 
    
    return (ambient + diffuse + specular); 
}