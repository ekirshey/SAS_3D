#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in uvec4 aBoneIndex;
layout (location = 4) in vec4 aBoneWeight;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 pv;
uniform mat4 m;

void main()
{
	TexCoords = aTexCoords;   
	FragPos = vec3(m * vec4(aPos.xyz, 1.0));
	Normal = mat3(transpose(inverse(m))) * aNormal.xyz;
    gl_Position = pv * vec4(FragPos, 1.0);
}