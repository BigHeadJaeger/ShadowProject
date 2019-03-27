#pragma once
#include<glm.hpp>
#include"Object.h"
using namespace glm;
namespace ProjPlaneShadow
{
	//void GetPlaneEquation(vec4 &planeEq, vec3 p1, vec3 p2, vec3 p3);

	mat4 GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L);
	//void MakePlanarShadowMatrix(mat4& proj, vec4 planeEq, vec3 vLightPos);

	//void PlaneProjProcess(Object& obj, Object& plane);
}