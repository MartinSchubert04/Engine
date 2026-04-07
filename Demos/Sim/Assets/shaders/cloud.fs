#version 460 core
out vec4 FragColor;

struct DirLight {
  vec3 direction;
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

uniform DirLight dirLight;

in vec3 vLocalPos;
in vec3 normal;

uniform samplerCube uCloudMap;

void main() {
  vec3 lightDir = normalize(-dirLight.direction);
  // sample cubemap
  vec3 tex = texture(uCloudMap, normalize(vLocalPos)).rgb;

  // convertir a máscara (blanco = nube)
  float mask = dot(tex, vec3(0.333));

  // eliminar fondo celeste
  mask = smoothstep(0.5, 0.8, mask);

  // iluminación simple
  float diff = max(dot(normalize(normal), lightDir), 0.0);

  vec3 cloudColor = vec3(1.0) * diff;

  FragColor = vec4(cloudColor, mask * 0.6);
  // FragColor = vec4(1.0);
}
