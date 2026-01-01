#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec4 aColor;
layout (location = 4) in float UseTexture;

out vec2 TexCoord;
out vec3 normal;
out vec3 FragPos;
out vec4 color;
out float useTexture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aTexCoords;    
    normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0));

    color = aColor;
    useTexture = UseTexture;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}