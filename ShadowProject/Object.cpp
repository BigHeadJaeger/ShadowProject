#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

/*void Object::CoorDataInit(mat4x4 world, mat4x4 worldviewproj, mat4x4 worldinvtranspose, vec3 scaler)
{
	World = world;
	WorldViewProj = worldviewproj;
	WorldInvTranspose = worldinvtranspose;
	Scaler = scaler;
}*/

void Object::CoorDataInit(vec3 pos , vec3 scaler , vec3 rot , float angle)
{
	Position = pos;
	Scaler = scaler;
	Rotation = rot;
	RotateAngle = angle;
}

void Object::SetObjMat(mat4 camView, mat4 camProj)
{
	World= translate(mat4(1.0), Position);
	World = scale(World, Scaler);
	World = rotate(World, RotateAngle, Rotation);
	WorldInvTranspose= transpose(inverse(World));
	WorldViewProj = camProj*camView*World;
	/*mat4x4 boxWorld = translate(mat4(1.0), vec3(0.0, 0.0, 0.0));
	mat4x4 boxWorldInvTranspose = transpose(inverse(boxWorld));
	mat4x4 boxWorldViewProj = camera.pro*camera.view*boxWorld;*/

}

void Object::InitUseMesh(string fileName, bool texture, ObjIndexType type)
{
	modelPath = fileName;

	indexType = type;

	BisTexture = texture;

	ifstream file;
	file.open(fileName);
	if (!file.is_open())
		cout << "打开obj文件失败" << endl;

	string line, flag;				//一行的内容和一行中开头的标志符

	float x, y, z;					//顶点坐标
	float u, v;						//纹理坐标
	float nx, ny, nz;				//法向量坐标

	char temp;

	//面上的三个点的 顶点、法向量、纹理索引
	unsigned indexV0, indexV1, indexV2, indexV3;
	unsigned indexN0, indexN1, indexN2, indexN3;
	unsigned indexT0, indexT1, indexT2, indexT3;

	//vector<float> vertices;
	//读取obj时进行临时存储
	vector<float> normals;
	vector<float> texcoords;

	//初始化AABB包围盒
	aabb.maxPos = vec3(float(-M_INFINITE), float(-M_INFINITE), float(-M_INFINITE));
	aabb.minPos = vec3(float(M_INFINITE), float(M_INFINITE), float(M_INFINITE));

	while (getline(file, line))
	{
		istringstream ist(line);
		ist >> flag;
		if (flag == "v")
		{
			ist >> x >> y >> z;

			if (x > aabb.maxPos.x)aabb.maxPos.x = x;
			if (y > aabb.maxPos.y)aabb.maxPos.y = y;
			if (z > aabb.maxPos.z)aabb.maxPos.z = z;

			if (x < aabb.minPos.x)aabb.minPos.x = x;
			if (y < aabb.minPos.y)aabb.minPos.y = y;
			if (z < aabb.minPos.z)aabb.minPos.z = z;

			//此处先将顶点位置放入pointsData，以保证顶点位置按照obj文件排列，之后再根据各索引将贴图法线坐标补全
			Point temp;
			temp.vertex = vec3(x, y, z);
			pointsData.push_back(temp);

			/*vertices.push_back(x);
			vertices.push_back(y);
			vertices.push_back(z);*/
		}
		else if (flag == "vt")
		{
			ist >> u >> v;
			texcoords.push_back(u);
			texcoords.push_back(v);
		}
		else if (flag == "vn")
		{
			ist >> nx >> ny >> nz;
			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);
		}
		else if (flag == "f")
		{
			if (type == ObjIndexType::Triangles)
			{
				//因为大部分obj文件不是每个顶点都将3个属性写一遍，多数是通过索引重复利用同一组数值(注：索引是从1开始的)
				if (BisTexture)			//区分是否有纹理坐标
				{
					ist >> indexV0 >> temp >> indexT0 >> temp >> indexN0
						>> indexV1 >> temp >> indexT1 >> temp >> indexN1
						>> indexV2 >> temp >> indexT2 >> temp >> indexN2;
					//渲染时用到的顺序
					renderIndex.push_back(indexV0 - 1);
					renderIndex.push_back(indexV1 - 1);
					renderIndex.push_back(indexV2 - 1);
					//根据索引将法线对应到之前只填了顶点位置的pointsData中
					pointsData[indexV0 - 1].normal = vec3(normals[3 * (indexN0 - 1)], normals[3 * (indexN0 - 1) + 1], normals[3 * (indexN0 - 1) + 2]);
					pointsData[indexV1 - 1].normal = vec3(normals[3 * (indexN1 - 1)], normals[3 * (indexN1 - 1) + 1], normals[3 * (indexN1 - 1) + 2]);
					pointsData[indexV2 - 1].normal = vec3(normals[3 * (indexN2 - 1)], normals[3 * (indexN2 - 1) + 1], normals[3 * (indexN2 - 1) + 2]);
					//填写纹理
					pointsData[indexV0 - 1].texcoord = vec2(texcoords[2 * (indexT0 - 1)], texcoords[2 * (indexT0 - 1) + 1]);
					pointsData[indexV1 - 1].texcoord = vec2(texcoords[2 * (indexT1 - 1)], texcoords[2 * (indexT1 - 1) + 1]);
					pointsData[indexV2 - 1].texcoord = vec2(texcoords[2 * (indexT2 - 1)], texcoords[2 * (indexT2 - 1) + 1]);

				}
				else
				{
					ist >> indexV0 >> temp >> temp >> indexN0
						>> indexV1 >> temp >> temp >> indexN1
						>> indexV2 >> temp >> temp >> indexN2;

					//渲染时用到的顺序
					renderIndex.push_back(indexV0 - 1);
					renderIndex.push_back(indexV1 - 1);
					renderIndex.push_back(indexV2 - 1);
					//根据索引将法线对应到之前只填了顶点位置的pointsData中
					pointsData[indexV0 - 1].normal = vec3(normals[3 * (indexN0 - 1)], normals[3 * (indexN0 - 1) + 1], normals[3 * (indexN0 - 1) + 2]);
					pointsData[indexV1 - 1].normal = vec3(normals[3 * (indexN1 - 1)], normals[3 * (indexN1 - 1) + 1], normals[3 * (indexN1 - 1) + 2]);
					pointsData[indexV2 - 1].normal = vec3(normals[3 * (indexN2 - 1)], normals[3 * (indexN2 - 1) + 1], normals[3 * (indexN2 - 1) + 2]);
				}

			}
			else if (type == ObjIndexType::Quad)
			{
				if (BisTexture)			//区分是否有纹理坐标
				{
					ist >> indexV0 >> temp >> indexT0 >> temp >> indexN0
						>> indexV1 >> temp >> indexT1 >> temp >> indexN1
						>> indexV2 >> temp >> indexT2 >> temp >> indexN2
						>> indexV3 >> temp >> indexT3 >> temp >> indexN3;

					renderIndex.push_back(indexV0 - 1);
					renderIndex.push_back(indexV1 - 1);
					renderIndex.push_back(indexV2 - 1);
					renderIndex.push_back(indexV3 - 1);

					//根据索引将法线对应到之前只填了顶点位置的pointsData中
					pointsData[indexV0 - 1].normal = vec3(normals[3 * (indexN0 - 1)], normals[3 * (indexN0 - 1) + 1], normals[3 * (indexN0 - 1) + 2]);
					pointsData[indexV1 - 1].normal = vec3(normals[3 * (indexN1 - 1)], normals[3 * (indexN1 - 1) + 1], normals[3 * (indexN1 - 1) + 2]);
					pointsData[indexV2 - 1].normal = vec3(normals[3 * (indexN2 - 1)], normals[3 * (indexN2 - 1) + 1], normals[3 * (indexN2 - 1) + 2]);
					pointsData[indexV3 - 1].normal = vec3(normals[3 * (indexN3 - 1)], normals[3 * (indexN3 - 1) + 1], normals[3 * (indexN3 - 1) + 2]);

					//填写纹理
					pointsData[indexV0 - 1].texcoord = vec2(texcoords[2 * (indexT0 - 1)], texcoords[2 * (indexT0 - 1) + 1]);
					pointsData[indexV1 - 1].texcoord = vec2(texcoords[2 * (indexT1 - 1)], texcoords[2 * (indexT1 - 1) + 1]);
					pointsData[indexV2 - 1].texcoord = vec2(texcoords[2 * (indexT2 - 1)], texcoords[2 * (indexT2 - 1) + 1]);
					pointsData[indexV3 - 1].texcoord = vec2(texcoords[2 * (indexT3 - 1)], texcoords[2 * (indexT3 - 1) + 1]);
				}
				else
				{
					ist >> indexV0 >> temp >> temp >> indexN0
						>> indexV1 >> temp >> temp >> indexN1
						>> indexV2 >> temp >> temp >> indexN2
						>> indexV3 >> temp >> temp >> indexN3;

					renderIndex.push_back(indexV0 - 1);
					renderIndex.push_back(indexV1 - 1);
					renderIndex.push_back(indexV2 - 1);
					renderIndex.push_back(indexV3 - 1);

					//根据索引将法线对应到之前只填了顶点位置的pointsData中
					pointsData[indexV0 - 1].normal = vec3(normals[3 * (indexN0 - 1)], normals[3 * (indexN0 - 1) + 1], normals[3 * (indexN0 - 1) + 2]);
					pointsData[indexV1 - 1].normal = vec3(normals[3 * (indexN1 - 1)], normals[3 * (indexN1 - 1) + 1], normals[3 * (indexN1 - 1) + 2]);
					pointsData[indexV2 - 1].normal = vec3(normals[3 * (indexN2 - 1)], normals[3 * (indexN2 - 1) + 1], normals[3 * (indexN2 - 1) + 2]);
					pointsData[indexV3 - 1].normal = vec3(normals[3 * (indexN3 - 1)], normals[3 * (indexN3 - 1) + 1], normals[3 * (indexN3 - 1) + 2]);

				}
			}

		}

	}

	file.close();

	ObjNormalize();
}

void Object::InitDirectBox(float width, float height, float depth)
{
	BisTexture = true;

	renderIndex.clear();
	pointsData.clear();

	renderIndex.resize(36);
	pointsData.resize(24);

	float halfW = width*0.5f;
	float halfH = height*0.5f;
	float halfD = depth*0.5f;

	//背面

	pointsData[0].vertex = vec3(-halfW, -halfH, -halfD);
	pointsData[0].normal = vec3(0.f, 0.f, -1.f);
	//pointsData[0].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[0].texcoord = vec2(0.f, 1.f);
	pointsData[1].vertex = vec3(-halfW, halfH, -halfD);
	pointsData[1].normal = vec3(0.f, 0.f, -1.f);
	//pointsData[1].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[1].texcoord = vec2(0.f, 0.f);
	pointsData[2].vertex = vec3(halfW, halfH, -halfD);
	pointsData[2].normal = vec3(0.f, 0.f, -1.f);
	//pointsData[2].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[2].texcoord = vec2(1.f, 0.f);
	pointsData[3].vertex = vec3(halfW, -halfH, -halfD);
	pointsData[3].normal = vec3(0.f, 0.f, -1.f);
	//pointsData[3].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[3].texcoord = vec2(1.f, 1.f);
	//左侧面
	pointsData[4].vertex = vec3(-halfW, -halfH, halfD);
	pointsData[4].normal = vec3(-1.f, 0.f, 0.f);
	//pointsData[4].tangent = vec3(0.f, 0.f, -1.f);
	pointsData[4].texcoord = vec2(0.f, 1.f);
	pointsData[5].vertex = vec3(-halfW, halfH, halfD);
	pointsData[5].normal = vec3(-1.f, 0.f, 0.f);
	//pointsData[5].tangent = vec3(0.f, 0.f, -1.f);
	pointsData[5].texcoord = vec2(0.f, 0.f);
	pointsData[6].vertex = vec3(-halfW, halfH, -halfD);
	pointsData[6].normal = vec3(-1.f, 0.f, 0.f);
	//pointsData[6].tangent = vec3(0.f, 0.f, -1.f);
	pointsData[6].texcoord = vec2(1.f, 0.f);
	pointsData[7].vertex = vec3(-halfW, -halfH, -halfD);
	pointsData[7].normal = vec3(-1.f, 0.f, 0.f);
	//pointsData[7].tangent = vec3(0.f, 0.f, -1.f);
	pointsData[7].texcoord = vec2(1.f, 1.f);
	//正面
	pointsData[8].vertex = vec3(halfW, -halfH, halfD);
	pointsData[8].normal = vec3(0.f, 0.f, 1.f);
	//pointsData[8].tangent = vec3(-1.f, 0.f, 0.f);
	pointsData[8].texcoord = vec2(0.f, 1.f);
	pointsData[9].vertex = vec3(halfW, halfH, halfD);
	pointsData[9].normal = vec3(0.f, 0.f, 1.f);
	//pointsData[9].tangent = vec3(-1.f, 0.f, 0.f);
	pointsData[9].texcoord = vec2(0.f, 0.f);
	pointsData[10].vertex = vec3(-halfW, halfH, halfD);
	pointsData[10].normal = vec3(0.f, 0.f, 1.f);
	//pointsData[10].tangent = vec3(-1.f, 0.f, 0.f);
	pointsData[10].texcoord = vec2(1.f, 0.f);
	pointsData[11].vertex = vec3(-halfW, -halfH, halfD);
	pointsData[11].normal = vec3(0.f, 0.f, 1.f);
	//pointsData[11].tangent = vec3(-1.f, 0.f, 0.f);
	pointsData[11].texcoord = vec2(1.f, 1.f);
	//右侧面
	pointsData[12].vertex = vec3(halfW, -halfH, -halfD);
	pointsData[12].normal = vec3(1.f, 0.f, 0.f);
	//pointsData[12].tangent = vec3(0.f, 0.f, 1.f);
	pointsData[12].texcoord = vec2(0.f, 1.f);
	pointsData[13].vertex = vec3(halfW, halfH, -halfD);
	pointsData[13].normal = vec3(1.f, 0.f, 0.f);
	//pointsData[13].tangent = vec3(0.f, 0.f, 1.f);
	pointsData[13].texcoord = vec2(0.f, 0.f);
	pointsData[14].vertex = vec3(halfW, halfH, halfD);
	pointsData[14].normal = vec3(1.f, 0.f, 0.f);
	//pointsData[14].tangent = vec3(0.f, 0.f, 1.f);
	pointsData[14].texcoord = vec2(1.f, 0.f);
	pointsData[15].vertex = vec3(halfW, -halfH, halfD);
	pointsData[15].normal = vec3(1.f, 0.f, 0.f);
	//pointsData[15].tangent = vec3(0.f, 0.f, 1.f);
	pointsData[15].texcoord = vec2(1.f, 1.f);
	//上面
	pointsData[16].vertex = vec3(-halfW, halfH, -halfD);
	pointsData[16].normal = vec3(0.f, 1.f, 0.f);
	//pointsData[16].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[16].texcoord = vec2(0.f, 1.f);
	pointsData[17].vertex = vec3(-halfW, halfH, halfD);
	pointsData[17].normal = vec3(0.f, 1.f, 0.f);
	//pointsData[17].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[17].texcoord = vec2(0.f, 0.f);
	pointsData[18].vertex = vec3(halfW, halfH, halfD);
	pointsData[18].normal = vec3(0.f, 1.f, 0.f);
	//pointsData[18].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[18].texcoord = vec2(1.f, 0.f);
	pointsData[19].vertex = vec3(halfW, halfH, -halfD);
	pointsData[19].normal = vec3(0.f, 1.f, 0.f);
	//pointsData[19].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[19].texcoord = vec2(1.f, 1.f);
	//底面
	pointsData[20].vertex = vec3(-halfW, -halfH, halfD);
	pointsData[20].normal = vec3(0.f, -1.f, 0.f);
	//pointsData[20].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[20].texcoord = vec2(0.f, 1.f);
	pointsData[21].vertex = vec3(-halfW, -halfH, -halfD);
	pointsData[21].normal = vec3(0.f, -1.f, 0.f);
	//pointsData[21].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[21].texcoord = vec2(0.f, 0.f);
	pointsData[22].vertex = vec3(halfW, -halfH, -halfD);
	pointsData[22].normal = vec3(0.f, -1.f, 0.f);
	//pointsData[22].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[22].texcoord = vec2(1.f, 0.f);
	pointsData[23].vertex = vec3(halfW, -halfH, halfD);
	pointsData[23].normal = vec3(0.f, -1.f, 0.f);
	//pointsData[23].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[23].texcoord = vec2(1.f, 1.f);

	//构建索引
	renderIndex[0] = 0;
	renderIndex[1] = 1;
	renderIndex[2] = 2;
	renderIndex[3] = 0;
	renderIndex[4] = 2;
	renderIndex[5] = 3;

	renderIndex[6] = 4;
	renderIndex[7] = 5;
	renderIndex[8] = 6;
	renderIndex[9] = 4;
	renderIndex[10] = 6;
	renderIndex[11] = 7;

	renderIndex[12] = 8;
	renderIndex[13] = 9;
	renderIndex[14] = 10;
	renderIndex[15] = 8;
	renderIndex[16] = 10;
	renderIndex[17] = 11;

	renderIndex[18] = 12;
	renderIndex[19] = 13;
	renderIndex[20] = 14;
	renderIndex[21] = 12;
	renderIndex[22] = 14;
	renderIndex[23] = 15;

	renderIndex[24] = 16;
	renderIndex[25] = 17;
	renderIndex[26] = 18;
	renderIndex[27] = 16;
	renderIndex[28] = 18;
	renderIndex[29] = 19;

	renderIndex[30] = 20;
	renderIndex[31] = 21;
	renderIndex[32] = 22;
	renderIndex[33] = 20;
	renderIndex[34] = 22;
	renderIndex[35] = 23;

}

void Object::InitDirectSphere(float radius, int slice, int stack)
{
	BisTexture = true;
	float PI = 3.14159265359;
	
	pointsData.clear();
	renderIndex.clear();

	int vertsPerRow = slice + 1;
	int nRows = stack - 1;

	int nVerts = vertsPerRow * nRows + 2;
	int nIndices = (nRows - 1)*slice * 6 + slice * 6;

	pointsData.resize(nVerts);
	renderIndex.resize(nIndices);

	for (int i = 1; i <= nRows; ++i)
	{
		float phy = PI * i / stack;
		float tmpRadius = radius * sin(phy);
		for (int j = 0; j < vertsPerRow; ++j)
		{
			float theta = 2 * PI * j / slice;
			int index = (i - 1)*vertsPerRow + j;

			float x = tmpRadius*cos(theta);
			float y = radius*cos(phy);
			float z = tmpRadius*sin(theta);

			//位置坐标
			pointsData[index].vertex = vec3(x, y, z);
			//法线
			vec3 N = vec3(x, y, z);
			pointsData[index].normal = N;
			//XMStoreFloat3(&pointsData[index].normal, XMVector3Normalize(N));
			//切线
			/*XMVECTOR T = XMVectorSet(-sin(theta), 0.f, cos(theta), 0.f);
			XMStoreFloat3(&pointsData[index].tangent, XMVector3Normalize(T));*/
			//纹理坐标
			pointsData[index].texcoord = vec2(j*1.f / slice, i*1.f / stack);
		}
	}

	int size = vertsPerRow * nRows;
	//添加顶部和底部两个顶点信息
	pointsData[size].vertex = vec3(0.f, radius, 0.f);
	pointsData[size].normal = vec3(0.f, 1.f, 0.f);
	//pointsData[size].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[size].texcoord = vec2(0.f, 0.f);

	pointsData[size + 1].vertex = vec3(0.f, -radius, 0.f);
	pointsData[size + 1].normal = vec3(0.f, -1.f, 0.f);
	//pointsData[size + 1].tangent = vec3(1.f, 0.f, 0.f);
	pointsData[size + 1].texcoord = vec2(0.f, 1.f);

	int tmp(0);
	int start1 = 0;
	int start2 = pointsData.size() - vertsPerRow - 2;
	int top = size;
	int bottom = size + 1;
	for (int i = 0; i < slice; ++i)
	{
		renderIndex[tmp] = top;
		renderIndex[tmp + 1] = start1 + i + 1;
		renderIndex[tmp + 2] = start1 + i;

		tmp += 3;
	}

	for (int i = 0; i < slice; ++i)
	{
		renderIndex[tmp] = bottom;
		renderIndex[tmp + 1] = start2 + i;
		renderIndex[tmp + 2] = start2 + i + 1;

		tmp += 3;
	}

	for (int i = 0; i < nRows - 1; ++i)
	{
		for (int j = 0; j < slice; ++j)
		{
			renderIndex[tmp] = i * vertsPerRow + j;
			renderIndex[tmp + 1] = (i + 1) * vertsPerRow + j + 1;
			renderIndex[tmp + 2] = (i + 1) * vertsPerRow + j;
			renderIndex[tmp + 3] = i * vertsPerRow + j;
			renderIndex[tmp + 4] = i * vertsPerRow + j + 1;
			renderIndex[tmp + 5] = (i + 1) * vertsPerRow + j + 1;

			tmp += 6;
		}
	}

}

void Object::initDirectGrid(float width, float height, int m, int n)
{
	BisTexture = true;
	renderIndex.clear();
	pointsData.clear();

	int nVertsRow = m + 1;			//x方向顶点数
	int nVertsCol = n + 1;			//z方向顶点数

	float oX = -width*0.5f;			//x方向的起始位置
	float oZ = height*0.5f;			//z方向的起始位置

	float dX = width / m;			//变化率
	float dZ = height / n;

	pointsData.resize(nVertsCol*nVertsRow);

	//从左上角开始逐行添加顶点信息
	for (int i = 0; i < nVertsCol; i++)
	{
		float tempZ = oZ - dZ*i;
		for (int j = 0; j < nVertsRow; j++)
		{
			int index = i*nVertsRow + j;
			pointsData[index].vertex.x = oX + dX*j;
			pointsData[index].vertex.y = 0.f;
			pointsData[index].vertex.z = tempZ;

			pointsData[index].normal = vec3(0.f, 1.f, 0.f);
			//pointsData[index].tangent = vec3(1.f, 0.f, 0.f);

			pointsData[index].texcoord = vec2(dX*j, dZ*i);
		}
	}

	renderIndex.resize(m*n * 6);			//一共m*n个格子，一个格子分为两个三角形则有6个顶点
	int temp = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			renderIndex[temp] = i*nVertsRow + j;
			renderIndex[temp + 1] = i*nVertsRow + j + 1;
			renderIndex[temp + 2] = (i + 1)*nVertsRow + j;
			renderIndex[temp + 3] = i*nVertsRow + j + 1;
			renderIndex[temp + 4] = (i + 1)*nVertsRow + j + 1;
			renderIndex[temp + 5] = (i + 1)*nVertsRow + j;

			temp += 6;
		}
	}

}

void Object::InitBuffers()
{
	//box
	//生成VAO，VAO记录的是一次绘制中所需要的信息
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//生成顶点buffer(VBO)并绑定
	//MyGeometryGenerator::GetInstance()->CreateBox(20.0f, 0.3f, 20.0f, box);
	//转换到一维数组中
	std::vector<float> vertexData;
	for (int i = 0; i < pointsData.size(); i++)
	{
		vertexData.push_back(pointsData[i].vertex.x);
		vertexData.push_back(pointsData[i].vertex.y);
		vertexData.push_back(pointsData[i].vertex.z);
		vertexData.push_back(pointsData[i].normal.x);
		vertexData.push_back(pointsData[i].normal.y);
		vertexData.push_back(pointsData[i].normal.z);
		if (BisTexture)
		{
			vertexData.push_back(pointsData[i].texcoord.x);		//
			vertexData.push_back(pointsData[i].texcoord.y);
		}
	}


	//创建顶点buffer
	glGenBuffers(1, &VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);					//先绑定，在用VAO传值时，就传送的是当前绑定的buffer
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), &vertexData[0], GL_STATIC_DRAW);		//指定数据
																											//创建索引buffer
	glGenBuffers(1, &IndexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, renderIndex.size() * sizeof(unsigned), &renderIndex[0], GL_STATIC_DRAW);

	//将顶点buffer中的数据指定到shader中
	//指定pos数据
	glEnableVertexAttribArray(0);										//开启索引为0的顶点属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);		//第五个代表取数据时的步长，最后一个代表在当前的一个步长内跳过前面多长的信息
																						//指定normal数据
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));		//每8个步长中的中间三个是法向量数据
																										//指定纹理坐标数据
	if (BisTexture)
	{
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	}

	glBindVertexArray(0);



}

void Object::TextureUseInfo(bool useTex, bool useAlbedo, bool useMetallic, bool useRoughness, bool useAo, bool useNormal, bool useShadowTex)
{
	BuseTexture = useTex;
	BAlbedo = useAlbedo;
	BMetallic = useMetallic;
	BRoughness = useRoughness;
	BAo = useAo;
	BNormal = useNormal;
	BShadowTex = useShadowTex;
}

void Object::LightUseInfo(bool receive)
{
	BReceiveLight = receive;
}

void Object::InitTexture(GLuint & TexID, string TexPath)
{
	glGenTextures(1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);		//此时绑定到了默认纹理单元0处
	//指定贴图方法
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//文件读取
	int width, height;
	unsigned char* pResult = SOIL_load_image(TexPath.c_str(), &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pResult);
	//生成一个mipmap
	glGenerateMipmap(GL_TEXTURE_2D);
	//解除绑定并释放
	glBindTexture(GL_TEXTURE_2D, 0);
	SOIL_free_image_data(pResult);

}

void Object::UseATexture(GLuint & TexID, GLuint & ID)
{
	TexID = ID;
}

void Object::InitMaterial(vec4 A, vec4 D, vec4 S, float SH)
{
	Material.ambient = A;
	Material.diffuse = D;
	Material.specular = S;
	Material.shiness = SH;
}

void Object::ObjNormalize()
{
	float X, Y, Z;
	X = max(fabs(aabb.maxPos.x), fabs(aabb.minPos.x));
	Y = max(fabs(aabb.maxPos.y), fabs(aabb.minPos.y));
	Z = max(fabs(aabb.maxPos.x), fabs(aabb.minPos.z));

	float result = 1.0f / max(X, max(Y, Z));

	for (int i = 0; i < pointsData.size(); i++)
	{
		pointsData[i].vertex *= result;
		//vertices[i] *= result;
	}
}
