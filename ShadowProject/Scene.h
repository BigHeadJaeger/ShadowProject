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
	//���ֳ�����Ϣ����������ʡ��ƹ⡢��������ĸ��־���

	//����
	Object Box;
	//Object BoxShadow;

	Object Sphere;

	Object Grid;

	Object Neptune;

	PhongDirectionalLight light1;


	GLuint frameBuffer;					//֡����
	GLuint depthTex;					//�������
	mat4 depthMVP;						//����ͶӰ�任����
	mat4 depthBiasMVP;					//����������������ľ���
	//bool testJudge;
	

public:
	//���
	MyCamera camera;


	//---------------------------------------------------------------------------------

private:
	//shader����
	ShaderProgram p1;

	ShaderProgram pShadowTex;


public:
	void Init();			//��ʼ��������Ϣ
	void Update();			//��Ҫ����ʱ��������־�����ʱ������shader�У�
	void Draw();			//���Ƴ���
private:

	//c++��shader֮��Ľ���
	void DrawElement(Object & obj, ShaderProgram&p);			//��������(����������������Ĵ��ݺ�����Ĵ��ݣ�����Ϊָ��ʹ����Щ��ͼ)
	void DrawDepthTexture(Object& obj, ShaderProgram&p);
	void SetTexture(GLuint & texId, int num, GLenum texNum, string samplerName, ShaderProgram&p);			//��ǰ�����ɺõ�Texture����shader

	void SetLight(PhongDirectionalLight&L, vec3 E, ShaderProgram&p);


	//mat4 GetShadowProjMatrix(Object& obj, vec3 lightPos);
	//mat4 GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L);


	//���ݲ�ͬ���͵�ֵ�����صķ�ʽ����shader��
	void SetUniform(string valueName, mat4x4& value, ShaderProgram&p);
	void SetUniform(string valueName, vec4& value, ShaderProgram&p);
	void SetUniform(string valueName, vec3& value, ShaderProgram&p);
	void SetUniform(string valueName, float value, ShaderProgram&p);

private:
										//��Ӱ�㷨ʵ�ַ���
	//ƽ����ӰͶӰ
	void PlaneShadowProcess(Object& obj, Object& plane);
	mat4 GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L);


	//��Ӱ��ͼ
	void STInit();						//��ʼ��֡�����Լ������󶨣���ʼ��ʱ����һ��
	void STConstructProj(vec3 Pos);		//�������մ�������Լ�ͶӰ���󣬲��ϸ���

	void NormalShadowMap();
};