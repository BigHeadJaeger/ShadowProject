#pragma once
#include"PublicStruct.h"
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//#include"Generate.h"
#include<vector>
#include <fstream>
#include <sstream>
#include<iostream>
#include<glew.h>
#include<SOIL.h>
using namespace std;



class Object
{
public:
	Object();
	~Object();

	void CoorDataInit(vec3 pos = vec3(0, 0, 0), vec3 scaler = vec3(1.f, 1.f, 1.f), vec3 rot = vec3(0.0, 1.f, 0.0), float angle = 0);
	void SetObjMat(mat4 camView, mat4 camProj);
	void InitUseMesh(string fileName, bool texture, ObjIndexType type);	//通过网格生成物体
	
	//通过指定坐标直接生成简单物体
	void InitDirectBox(float width, float height, float depth);														//直接通过几何的方式生成物体
	void InitDirectSphere(float radius, int slice, int stack);
	void initDirectGrid(float width, float height, int m, int n);

	void InitBuffers();										//设置VAO和VBO
	void TextureUseInfo(bool useTex = true, bool useAlbedo = false,
		bool useMetallic = false, bool useRoughness = false, bool useAo = false,
		bool useNormal = false, bool useShadowTex = false);							//用来指明物体是否使用纹理以及具体使用哪些纹理
	void LightUseInfo(bool receive);
	void InitTexture(GLuint& TexID, string TexPath);		//生成纹理对象并绑定数据(将图片转化为纹理数据，根据不同的ID设置相应的纹理)
	void UseATexture(GLuint& TexID, GLuint&ID);				//使用一个已经生成好的纹理对象
	void InitMaterial(vec4 A, vec4 D, vec4 S, float SH);

private:
	void ObjNormalize();													//网格数据归一化

public:
	vector<Point> pointsData;												//物体顶点数据
	vector<unsigned> renderIndex;											//存放面上顶点的索引
	bool BisTexture;											//只是用来表示物体的顶点信息含有纹理坐标，并不是一定要使用纹理
	ObjIndexType indexType;

	//物体的坐标属性
	mat4x4 World;					//世界矩阵
	mat4x4 WorldViewProj;			//最终坐标转换矩阵
	mat4x4 WorldInvTranspose;		//用来将法向量转换到世界空间
	vec3 Scaler;					//物体的放大系数
	vec3 Position;					//物体的位置
	vec3 Rotation;					//物体的旋转
	float RotateAngle;				//物体旋转的角度


						//物体的VAO、VBO编号
	GLuint VAO;
	GLuint VertexBuffer;
	GLuint IndexBuffer;


						//物体的贴图编号（不一定全都需要）
	bool BuseTexture;
	GLuint TAlbedo;					//反射贴图（基础颜色）
	bool BAlbedo;
	GLuint TMetallic;				//金属度贴图
	bool BMetallic;
	GLuint TRoughness;				//粗糙贴图
	bool BRoughness;
	GLuint TAo;						//环境贴图
	bool BAo;
	GLuint TNormal;					//法线贴图
	bool BNormal;
	GLuint TShadowTex;				//阴影贴图
	bool BShadowTex;

	ObjMaterial Material;			//物体材质属性，在使用简单光照时需要指定
	bool BReceiveLight;					//物体是否接收光照

private:
	//使用网格生成物体的时候记录以下变量
	string modelPath;

	AABB aabb;
};