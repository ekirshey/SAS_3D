#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in uvec4 aBoneIndex;
layout (location = 4) in vec4 aBoneWeight;

out vec4 VertexColor;

uniform mat4 pvm;

void main()
{
	VertexColor = vec4(1.0, 0.0, 0.0, 1.0);
	gl_Position = pvm * vec4(aPos, 1.0);
}