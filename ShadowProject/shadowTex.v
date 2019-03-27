#version 430

layout(location=0) in vec3 VertexPosition;
layout(location=1) in vec3 normal;
layout(location=2) in vec2 texCoord;


uniform mat4 world;
uniform mat4 depthMVP;



void main()
{
	gl_Position=depthMVP*world*vec4(VertexPosition,1.0);
}