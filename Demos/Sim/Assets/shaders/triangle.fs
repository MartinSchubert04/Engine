#version 460 core

in vec3 Position;
in vec4 Color;

out vec4 FragColor;

void main() {
  FragColor = vec4(Color);
}
