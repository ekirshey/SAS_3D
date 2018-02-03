#version 330 core
layout (location = 0) in vec3	aPos;
layout (location = 1) in vec3	aNormal;
layout (location = 2) in vec2	aTexCoords;
layout (location = 3) in uvec4	aBoneIndex;
layout (location = 4) in vec4	aBoneWeight;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 Bones[100];  //Array of bones that you compute (animate) on the CPU and you upload to the shader
uniform mat4 pv;
uniform mat4 m;

void main()
{
	vec4 newVertex;
	vec4 newNormal;
	vec4 norm = vec4(aNormal.xyz, 0.0);
	vec4 pos = vec4(aPos.xyz, 1.0);
	uint index;
	//-----------------------
	index=aBoneIndex.x;
	newVertex = (Bones[index] * pos)  * aBoneWeight.x;	
	newNormal = (Bones[index] * norm) * aBoneWeight.x;	
	index=aBoneIndex.y;
	newVertex = (Bones[index] * pos)  * aBoneWeight.y + newVertex;
	newNormal = (Bones[index] * norm) * aBoneWeight.y + newNormal;
	index=aBoneIndex.z;
	newVertex = (Bones[index] * pos)  * aBoneWeight.z + newVertex;
	newNormal = (Bones[index] * norm) * aBoneWeight.z + newNormal;
	index=aBoneIndex.w;
	newVertex = (Bones[index] * pos)  * aBoneWeight.w + newVertex;
	newNormal = (Bones[index] * norm) * aBoneWeight.w + newNormal;
	
    TexCoords = aTexCoords;   
	FragPos = vec3(m * vec4(newVertex.xyz, 1.0));
	Normal = mat3(transpose(inverse(m))) * newNormal.xyz;
    gl_Position = pv * vec4(FragPos, 1.0);
}