#version 430 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in uvec4 aBoneIndex;
layout (location = 4) in vec4 aBoneWeight;

uniform mat4 Bones[100];  //Array of bones that you compute (animate) on the CPU and you upload to the shader
uniform mat4 pvm;

out vec2 TexCoords;

void main()
{
	vec4 newVertex;
	vec4 newNormal;
	vec4 pos = vec4(aPos.xyz, 1.0);
	uint index;
	//-----------------------
	index=aBoneIndex.x;
	newVertex = (Bones[index] * pos);// * aBoneWeight.x;	
	//index=aBoneIndex[1];
	//newVertex = (Bones[index] * pos) * aBoneWeight[1] + newVertex;
	//index=aBoneIndex[2];
	//newVertex = (Bones[index] * pos) * aBoneWeight[2] + newVertex;
	//index=aBoneIndex[3];
	//newVertex = (Bones[index] * pos) * aBoneWeight[3] + newVertex;
	
	
    TexCoords = aTexCoords;    
    gl_Position = pvm * pos;//vec4(newVertex.xyz, 1.0);
}