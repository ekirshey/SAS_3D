#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in uvec4 aBoneIndex;
layout (location = 4) in vec4 aBoneWeight;

out vec2 TexCoords;

uniform mat4 pvm;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = pvm * vec4(aPos.xyz, 1.0);
}