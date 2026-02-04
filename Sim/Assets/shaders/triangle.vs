#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec4 aColor;

out vec3 Position;
out vec4 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  Position = aPos;
  Color = aColor;

  gl_Position = projection * view * model * vec4(aPos, 1.0);
}
