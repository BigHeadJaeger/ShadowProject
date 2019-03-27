#pragma once
#include<glm.hpp>
using namespace glm;
class PhongLight
{
public:
	vec4 Ambient;
	vec4 Diffuse;
	vec4 Specular;
	vec3 Intensity;

	PhongLight():Ambient(vec4(0)),Diffuse(vec4(0)),Specular(vec4(0)), Intensity(vec3(0)){}
};

class PhongDirectionalLight:public PhongLight
{
public:
	vec4 Direction;
public:
	PhongDirectionalLight():PhongLight(), Direction(vec4(0)){}

	void InitLight(vec4 A=vec4(0), vec4 D = vec4(0), vec4 S = vec4(0), vec4 Dir = vec4(0,0,0,0),vec3 I = vec3(0))
	{
		Ambient = A;
		Diffuse = D;
		Specular = S;
		Direction = Dir;
		Intensity = I;
	}
};

class PhongPointLight :public PhongLight
{
public:
	vec4 Position;				//点光源位置
	float Range;				//范围
	vec3 att;					//	
};