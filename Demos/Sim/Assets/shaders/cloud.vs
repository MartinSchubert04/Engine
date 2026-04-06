#version 460 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec4 aColor;
layout(location = 4) in float UseTexture;

out vec3 vLocalPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  // posición en espacio mundo
  vec4 worldPos = model * vec4(aPos, 1.0);

  // dirección para cubemap
  vLocalPos = aPos;

  // normal transformada
  normal = mat3(transpose(inverse(model))) * aNormal;

  gl_Position = projection * view * worldPos;
}
