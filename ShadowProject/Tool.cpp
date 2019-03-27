#include "Tool.h"

vec3 GetNorFromPlan(vec3 p1, vec3 p2, vec3 p3)
{
	vec3 v1, v2;
	v1 = p2 - p1;
	v2 = p3 - p1;

	vec3 result = cross(v1, v2);
	result = normalize(result);

	return result;
}
