#include "Camera.h"

MyCamera::MyCamera()
{
	lookDir = vec3(0.0f, 0.0f, -1.0f);
	lookRight = vec3(1.0f, 0.0f, 0.0f);
	lookUp = vec3(0.0f, 1.0f, 0.0f);
	eyePos = vec3(0.0, 0.0, 0.0);
}

void MyCamera::Init(vec3 pos, vec3 point)
{
	eyePos = pos;
	lookAtPoint = point;

	lookDir = normalize(lookAtPoint - eyePos);

	//计算up
	lookLeft = cross(vec3(0.0, 1.0, 0.0), lookDir);
	up = cross(lookDir, lookLeft);
}

void MyCamera::SetView()
{
	//lookDir = normalize(lookAtPoint - eyePos);
	//lookAtPoint用eyepos加上lookDir即可
	view = lookAt(eyePos, lookAtPoint, up);
	//view *= rotate(mat4(1.0f), 45.0f, vec3(0.0, 1.0, 0.0));
}

void MyCamera::SetPro()
{
	pro = perspective(45.0f, ((float)1200) / (1000), 0.1f, 1000.0f);
}

void MyCamera::SetOrtho(float left, float right, float bottom, float up, float near, float far)
{
	pro = ortho(left, right, bottom, up, near, far);
}

void MyCamera::Walk(float dis)
{
	//获取眼睛看向的方向
	//lookDir = normalize(lookAtPoint - eyePos);

	eyePos += lookDir*vec3(dis);		//lookDir决定当前前后走动时对三个轴的贡献，与dis相乘加到eyepos上
	lookAtPoint += lookDir*vec3(dis);
}

void MyCamera::LRMove(float dis)
{
	//lookRight = normalize(cross(lookDir, lookUp));

	eyePos += lookLeft*vec3(dis);
	lookAtPoint += lookLeft*vec3(dis);
}

void MyCamera::LRRotate(float dis)
{
	//lookDir = normalize(lookAtPoint - eyePos);
	float dist = length(lookAtPoint - eyePos);
	mat4 ro = rotate(mat4(1.0f), dis, vec3(0.0, 1.0f, 0.0));

	lookDir = (ro*vec4(lookDir, 1.0f));

	lookDir = normalize(lookDir);

	lookAtPoint = lookDir*dist;

}
