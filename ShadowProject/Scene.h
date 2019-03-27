#pragma once


#include"Program.h"
#include"Camera.h"
#include"Object.h"
#include"SampleLights.h"

#include"Tool.h"
using namespace glm;
class MyScene
{
private:
	//各种场景信息（相机、材质、灯光、各种物体的各种矩阵）

	//物体
	Object Box;
	//Object BoxShadow;

	Object Sphere;

	Object Grid;

	Object Neptune;

	PhongDirectionalLight light1;


	GLuint frameBuffer;					//帧缓冲
	GLuint depthTex;					//深度纹理
	mat4 depthMVP;						//正交投影变换矩阵
	mat4 depthBiasMVP;					//用于纹理的修正过的矩阵
	//bool testJudge;
	

public:
	//相机
	MyCamera camera;


	//---------------------------------------------------------------------------------

private:
	//shader程序
	ShaderProgram p1;

	ShaderProgram pShadowTex;


public:
	void Init();			//初始化各种信息
	void Update();			//需要动画时，计算各种矩阵（暂时不传入shader中）
	void Draw();			//绘制场景
private:

	//c++与shader之间的交互
	void DrawElement(Object & obj, ShaderProgram&p);			//绘制物体(里面包括物体各矩阵的传递和纹理的传递，参数为指定使用哪些贴图)
	void DrawDepthTexture(Object& obj, ShaderProgram&p);
	void SetTexture(GLuint & texId, int num, GLenum texNum, string samplerName, ShaderProgram&p);			//将前面生成好的Texture传入shader

	void SetLight(PhongDirectionalLight&L, vec3 E, ShaderProgram&p);


	//mat4 GetShadowProjMatrix(Object& obj, vec3 lightPos);
	//mat4 GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L);


	//根据不同类型的值用重载的方式传入shader中
	void SetUniform(string valueName, mat4x4& value, ShaderProgram&p);
	void SetUniform(string valueName, vec4& value, ShaderProgram&p);
	void SetUniform(string valueName, vec3& value, ShaderProgram&p);
	void SetUniform(string valueName, float value, ShaderProgram&p);

private:
										//阴影算法实现方法
	//平面阴影投影
	void PlaneShadowProcess(Object& obj, Object& plane);
	mat4 GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L);


	//阴影贴图
	void STInit();						//初始化帧缓冲以及纹理并绑定，初始化时调用一次
	void STConstructProj(vec3 Pos);		//构建光照处的相机以及投影矩阵，不断更新

	void NormalShadowMap();
};