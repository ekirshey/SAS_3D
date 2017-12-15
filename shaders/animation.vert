#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in int aBoneIndex[4];
layout (location = 4) in float aBoneWeight[4];

out vec2 TexCoords;

uniform mat4 mvp;
uniform mat4 Bones[100];  //Array of bones that you compute (animate) on the CPU and you upload to the shader

void main()
{
	vec4 newVertex;
	vec4 newNormal;
	vec4 pos = vec4(aPos.xyz, 1.0);
	int index;
	//-----------------------
	index=aBoneIndex[0];
	newVertex = (Bones[index] * pos) * aBoneWeight[0];
	newNormal = (Bones[index] * vec4(aNormal, 0.0)) * aBoneWeight[0];
	index=aBoneIndex[1];
	newVertex = (Bones[index] * pos) * aBoneWeight[1] + newVertex;
	newNormal = (Bones[index] * vec4(aNormal, 0.0)) * aBoneWeight[1] + newNormal;
	index=aBoneIndex[2];
	newVertex = (Bones[index] * pos) * aBoneWeight[2] + newVertex;
	newNormal = (Bones[index] * vec4(aNormal, 0.0)) * aBoneWeight[2] + newNormal;
	index=aBoneIndex[3];
	newVertex = (Bones[index] * pos) * aBoneWeight[3] + newVertex;
	newNormal = (Bones[index] * vec4(aNormal, 0.0)) * aBoneWeight[3] + newNormal;
    TexCoords = aTexCoords;    
    gl_Position = mvp * vec4(newVertex.xyz, 1.0);
}