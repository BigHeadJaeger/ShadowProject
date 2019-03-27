#include "ProjPlaneShadow.h"

/*void ProjPlaneShadow::GetPlaneEquation(vec4 & planeEq, vec3 p1, vec3 p2, vec3 p3)
{
	// Get two vectors... do the cross product
	vec3 v1, v2;

	// V1 = p3 - p1
	v1 = p3 - p1;
	v1[0] = p3[0] - p1[0];
	v1[1] = p3[1] - p1[1];
	v1[2] = p3[2] - p1[2];

	// V2 = P2 - p1
	v2 = p2 - p1;
	v2[0] = p2[0] - p1[0];
	v2[1] = p2[1] - p1[1];
	v2[2] = p2[2] - p1[2];

	// Unit normal to plane - Not sure which is the best way here
	planeEq = vec4(cross(v1, v2), 0.0);
	*m3dCrossProduct3(planeEq, v1, v2);
	m3dNormalizeVector3(planeEq);
	normalize(planeEq);
	// Back substitute to get D
	planeEq[3] = -(planeEq[0] * p3[0] + planeEq[1] * p3[1] + planeEq[2] * p3[2]);
}*/

mat4 ProjPlaneShadow::GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L)
{
	vec3& l = vec3(L.x, L.y, L.z);

	float d = -glm::dot(x0_, n_);
	//LOG(INFO) << "the d " << d;
	vec3& n = n_;
	vec4 c0(glm::dot(n, l) + d*L.w - n.x * l.x, -(n.x * l.y), -n.x * l.z, -n.x*L.w);
	vec4 c1(-n.y * l.x, glm::dot(n, l) + d*L.w - n.y * l.y, -n.y*l.z, -n.y*L.w);
	vec4 c2(-n.z*l.x, -n.z*l.y, glm::dot(n, l) + d*L.w - n.z*l.z, -n.z*L.w);
	vec4 c3(-d * l.x, -d*l.y, -d*l.z, glm::dot(n, l));
	mat4 mat(c0, c1, c2, c3);
	return mat;
}

/*void ProjPlaneShadow::MakePlanarShadowMatrix(mat4 & proj, vec4 planeEq, vec3 vLightPos)
{
	// These just make the code below easier to read. They will be
	// removed by the optimizer.
	float a = planeEq[0];
	float b = planeEq[1];
	float c = planeEq[2];
	float d = planeEq[3];

	float dx = -vLightPos[0];
	float dy = -vLightPos[1];
	float dz = -vLightPos[2];

	// Now build the projection matrix

	proj[0][0] = b * dy + c * dz;
	proj[0][1] = -a * dy;
	proj[0][2] = -a * dz;
	proj[0][3] = 0.0;

	proj[1][0] = -b * dx;
	proj[1][1] = a * dx + c * dz;
	proj[1][2] = -b * dz;
	proj[1][3] = 0.0;

	proj[2][0] = -c * dx;
	proj[2][1] = -c * dy;
	proj[2][2] = a * dx + b * dy;
	proj[2][3] = 0.0;

	proj[2][0] = -d * dx;
	proj[2][1] = -d * dy;
	proj[2][2] = -d * dz;
	proj[2][3] = a * dx + b * dy + c * dz;
	// Shadow matrix ready
}*/

