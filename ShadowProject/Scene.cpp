#include "Scene.h"

void MyScene::Init()
{
	//��ɫ����ʼ��
	glewInit();
	p1.SetShader("shader.v", "shader.f");

	pShadowTex.SetShader("shadowTex.v", "shadowTex.f");

	//�����Ϣ��ʼ��

	camera.Init(vec3(0, 2, 8), vec3(0, 0, 0));

	//���ճ�ʼ��
	light1.InitLight(vec4(0.6f, 0.6f, 0.6f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(1, -1.0, 1, 0), vec3(1.5, 1.5, 1.5));

	

	//�����ʼ��
	Box.InitDirectBox(0.2, 1, 1);					//���㡢������Ϣ��ʼ��
	Box.InitBuffers();							//�����ʼ��
	Box.CoorDataInit(vec3(-2.0, 1.0, 0.0),vec3(1.0,3.0,8.0));
	Box.TextureUseInfo(true, false);				//����ʹ����Ϣ��ʼ��
	Box.InitTexture(Box.TAlbedo, "Material\\rusty\\rustediron2_basecolor.png");		//������Ҫ������
	Box.LightUseInfo(true);						//�����Ƿ���չ���
	Box.InitMaterial(vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(0.5, 0.5, 0.5, 1.0f), vec4(0.7f, 0.7f, 0.7f, 1.0f), 7.0f);

	/*Sphere.InitDirectSphere(1, 60, 60);
	Sphere.InitBuffers();
	Sphere.InitTexture(Sphere.AlbedoT, "Material\\oakfloor\\oakfloor_basecolor.png");*/

	Sphere.InitDirectSphere(1,20,20);					//���㡢������Ϣ��ʼ��
	Sphere.InitBuffers();							//�����ʼ��
	Sphere.CoorDataInit(vec3(0.0, 1.5, 0.0));
	Sphere.TextureUseInfo(true, false);				//����ʹ����Ϣ��ʼ��
	Sphere.InitTexture(Sphere.TAlbedo, "Material\\rusty\\rustediron2_basecolor.png");		//������Ҫ������
	Sphere.LightUseInfo(true);						//�����Ƿ���չ���
	Sphere.InitMaterial(vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(0.5, 0.5, 0.5, 1.0f), vec4(0.7f, 0.7f, 0.7f, 1.0f), 7.0f);



	Grid.initDirectGrid(1, 1, 10, 10);
	Grid.InitBuffers();
	Grid.CoorDataInit(vec3(0.0, -0.5, 0.0), vec3(10, 10, 10));
	Grid.TextureUseInfo(true, true);
	Grid.InitTexture(Grid.TAlbedo, "Material\\oakfloor\\oakfloor_basecolor.png");
	Grid.LightUseInfo(true);
	Grid.InitMaterial(vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(1.f, 1.f, 1.f, 1.0f), vec4(0.3f, 0.3f, 0.3f, 1.0f), 14.0f);

	

	Neptune.InitUseMesh("OBJ//Neptune.obj", true, ObjIndexType::Triangles);
	Neptune.InitBuffers();
	Neptune.CoorDataInit(vec3(2.0, 1.5, 0.0), vec3(2.0), vec3(0.0, 1.0, 0.0), 3.1415);
	Neptune.TextureUseInfo(true);
	Neptune.LightUseInfo(true);
	Neptune.InitMaterial(vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(1.f, 1.f, 1.f, 1.0f), vec4(0.3f, 0.3f, 0.3f, 1.0f), 14.0f);
	


	STInit();		//��ʼ����Ӱ��ͼ


}

void MyScene::Update()					//�Ը��־�����г�ʼ�����߸���(ʵ�ֶ���Ч��)
{
	camera.SetView();
	camera.SetPro();

	Box.SetObjMat(camera.view, camera.pro);
	/*mat4x4 boxWorld = translate(mat4(1.0), vec3(0.0, 0.0, 0.0));
	mat4x4 boxWorldInvTranspose = transpose(inverse(boxWorld));
	mat4x4 boxWorldViewProj = camera.pro*camera.view*boxWorld;
	Box.CoorDataInit(boxWorld, boxWorldViewProj, boxWorldInvTranspose);*/

	Sphere.SetObjMat(camera.view, camera.pro);

	Grid.SetObjMat(camera.view, camera.pro);

	Neptune.SetObjMat(camera.view, camera.pro);
	/*vec3 scaler = vec3(10, 10, 10);
	mat4x4 gridWorld = translate(mat4(1.0), vec3(0.0, -0.55, 0.0));
	gridWorld = scale(gridWorld, scaler);
	mat4x4 gridWorldInvTranspose = transpose(inverse(gridWorld));
	mat4x4 gridWorldViewProj = camera.pro*camera.view*gridWorld;
	Grid.CoorDataInit(gridWorld, gridWorldViewProj, gridWorldInvTranspose, scaler);*/
	/*mat4x4 sphereWorld = translate(mat4(1.0), vec3(0.0, 0, -2));
	mat4x4 sphereWorldInvTranspose = transpose(inverse(sphereWorld));
	mat4x4 sphereWorldViewProj = camera.pro*camera.view*sphereWorld;
	Sphere.CoorDataInit(sphereWorld, sphereWorldViewProj, sphereWorldInvTranspose);*/

	/*GLuint useLight = glGetUniformLocation(p1.p, "useLight");
	glUniform1i(useLight, true);*/
	SetLight(light1, camera.eyePos,p1);


	STConstructProj(-vec3(light1.Direction.x, light1.Direction.y, light1.Direction.z));		//������Ӱ��ͼͶӰ����
	//STConstructProj(camera.eyePos);		//������Ӱ��ͼͶӰ����

}

void MyScene::Draw()
{
	//��������Ⱦ������
	/*if (testJudge)
	{
		ShadowTextureProcess();			//������Ӱ��ͼ�����г�ʼ��
		testJudge = false;
	}*/

	NormalShadowMap();


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);					//��ά����Ҫ���������޳�
	glUseProgram(p1.p);						//������ɫ������
	//glFrontFace(GL_CW);
	//glCullFace(GL_FRONT);

	Grid.BShadowTex = true;
	Grid.UseATexture(Grid.TShadowTex, depthTex);
	//Grid.UseATexture(Grid.TAlbedo, depthTex);
	DrawElement(Grid, p1);

	Box.BShadowTex = true;
	Box.UseATexture(Box.TShadowTex, depthTex);
	DrawElement(Box, p1);

	Sphere.BShadowTex = true;
	Sphere.UseATexture(Sphere.TShadowTex, depthTex);
	DrawElement(Sphere, p1);

	Neptune.BShadowTex = true;
	Neptune.UseATexture(Neptune.TShadowTex, depthTex);
	DrawElement(Neptune, p1);



	//PlaneShadowProcess(Box, Grid);
	//PlaneShadowProcess(Sphere, Grid);
	//PlaneShadowProcess(Neptune, Grid);

	/*glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);					//��ά����Ҫ���������޳�
	//glUseProgram(p1.p);						//������ɫ������
	glUseProgram(pShadowTex.p);
	DrawDepthTexture(Grid);
	DrawDepthTexture(Box);
	/*DrawElement(Grid);

	DrawElement(Box);*/

}

void MyScene::DrawElement(Object & obj, ShaderProgram&p)
{
	SetUniform("depthBiasMVP", depthBiasMVP, p);


	//GLuint tempJudge;
										
	glBindVertexArray(obj.VAO);				//��ǰ�����úõ�VAO
											//��������任����
	SetUniform("worldViewProj", obj.WorldViewProj, p);
	SetUniform("world", obj.World, p);
	SetUniform("worldInvTranspose", obj.WorldInvTranspose, p);
	


	//���ݲ����϶������ѡ�񣬽���Ҫ����������ɫ��
	//�Ƚ��Ƿ�ʹ��������shader
	SetUniform("useTexture", obj.BuseTexture, p1);
	//tempJudge = glGetUniformLocation(p.p, "useTexture");
	//glUniform1i(tempJudge, obj.BuseTexture);
	if (obj.BuseTexture)
	{
		//������ʹ������������������
		if (obj.BisTexture)
		{
			//����������ͼ
			SetUniform("useAlbedo", obj.BAlbedo, p1);
			//tempJudge = glGetUniformLocation(p.p, "useAlbedo");
			//glUniform1i(tempJudge, obj.BAlbedo);
			if (obj.BAlbedo)
			{
				SetTexture(obj.TAlbedo, 0, GL_TEXTURE0, "albedoMap", p);

			}

		}

		//��Ӱ��ͼ����������������ͬһ�����ͣ�����Ҫ����������������꣩
		SetUniform("useShadowTex", obj.BShadowTex, p1);
		if (obj.BShadowTex)
		{
			SetTexture(obj.TShadowTex, 1, GL_TEXTURE1, "shadowTex", p1);
		}
	}

	//�������Ƿ���չ��մ���shader
	GLuint useLight = glGetUniformLocation(p.p, "useLight");
	glUniform1i(useLight, obj.BReceiveLight);
	if (obj.BReceiveLight)
	{
		SetUniform("material.ambient", obj.Material.ambient, p);
		SetUniform("material.diffuse", obj.Material.diffuse, p);
		SetUniform("material.specular", obj.Material.specular, p);
		SetUniform("material.shiness", obj.Material.shiness, p);
	}




	if (obj.indexType == ObjIndexType::Triangles)
		glDrawElements(GL_TRIANGLES, obj.renderIndex.size(), GL_UNSIGNED_INT, 0);
	else if (obj.indexType == ObjIndexType::Quad)
		glDrawElements(GL_QUADS, obj.renderIndex.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void MyScene::DrawDepthTexture(Object & obj, ShaderProgram&p)
{

	glBindVertexArray(obj.VAO);				//��ǰ�����úõ�VAO

	SetUniform("world", obj.World, p);
	SetUniform("depthMVP", depthMVP, p);

	if (obj.indexType == ObjIndexType::Triangles)
		glDrawElements(GL_TRIANGLES, obj.renderIndex.size(), GL_UNSIGNED_INT, 0);
	else if (obj.indexType == ObjIndexType::Quad)
		glDrawElements(GL_QUADS, obj.renderIndex.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void MyScene::SetTexture(GLuint & texId,int num,GLenum texNum,string samplerName,ShaderProgram&p)//����������󡢲�������Ӧ������Ԫ��Ҫ���������Ԫ�������������֡�shader����
{
	GLuint texLocation;
	glActiveTexture(texNum);							//��������Ԫ(����λ��)0��
	glBindTexture(GL_TEXTURE_2D, texId);				//���������󶨵���ǰ���������Ԫ��
	//������ָ����������Ӧ�ĸ�����Ԫ
	texLocation = glGetUniformLocation(p.p, samplerName.c_str());	//��ȡ��������location
	glUniform1i(texLocation, num);									//ָ����������Ӧ��ǰ�󶨵�����Ԫ0

}

void MyScene::SetLight(PhongDirectionalLight & L, vec3 E,ShaderProgram&p)
{
	SetUniform("dirLight.ambient", L.Ambient, p);
	SetUniform("dirLight.diffuse", L.Diffuse, p);
	SetUniform("dirLight.specular", L.Specular, p);
	SetUniform("dirLight.direction", L.Direction, p);
	SetUniform("dirLight.intensity", L.Intensity, p);
	SetUniform("eyePos", E, p);
}

/*mat4 MyScene::GetShadowProjMatrix(Object & plane, vec3 lightPos)
{
	vec4 planeFun;
	//ProjPlaneShadow::GetPlaneEquation(planeFun, plane.pointsData[0].vertex, plane.pointsData[1].vertex, plane.pointsData[20].vertex);
	ProjPlaneShadow::GetPlaneEquation(planeFun, vec3(0,0,0), vec3(1, 0, 0), vec3(0, 0, 1));
	mat4 result;
	ProjPlaneShadow::MakePlanarShadowMatrix(result, planeFun, lightPos);
	return result;
}*/

/*mat4 MyScene::GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L)
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

}*/

void MyScene::SetUniform(string valueName, mat4x4 &value, ShaderProgram&p)
{
	GLuint location;
	location = glGetUniformLocation(p.p, valueName.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(value));
}

void MyScene::SetUniform(string valueName, vec4 & value, ShaderProgram&p)
{
	GLuint location;
	location = glGetUniformLocation(p.p, valueName.c_str());
	glUniform4fv(location, 1, value_ptr(value));
}

void MyScene::SetUniform(string valueName, vec3 & value, ShaderProgram&p)
{
	GLuint location;
	location = glGetUniformLocation(p.p, valueName.c_str());
	glUniform3fv(location, 1, value_ptr(value));
}

void MyScene::SetUniform(string valueName, float value, ShaderProgram&p)
{
	GLuint location;
	location = glGetUniformLocation(p.p, valueName.c_str());
	glUniform1f(location, value);
}

void MyScene::PlaneShadowProcess(Object & obj, Object & plane)
{
	vec3 planNor = GetNorFromPlan(plane.pointsData[0].vertex, plane.pointsData[2].vertex, plane.pointsData[23].vertex);
	mat4 proj = GetShadowProjMatrix2(plane.Position + (planNor / 100.f), planNor, -light1.Direction);

	SetUniform("planeShadowProj", proj, p1);
	SetUniform("isPlaneShadow", true, p1);

	DrawElement(obj,p1);
	SetUniform("isPlaneShadow", false, p1);				//�ָ�Ϊ���������������Ӱ
}

mat4 MyScene::GetShadowProjMatrix2(vec3 x0_, vec3 n_, vec4 L)
{
	vec3& l = vec3(L.x, L.y, L.z);

	float d = -glm::dot(x0_, n_);

	vec3& n = n_;
	vec4 c0(glm::dot(n, l) + d*L.w - n.x * l.x, -(n.x * l.y), -n.x * l.z, -n.x*L.w);
	vec4 c1(-n.y * l.x, glm::dot(n, l) + d*L.w - n.y * l.y, -n.y*l.z, -n.y*L.w);
	vec4 c2(-n.z*l.x, -n.z*l.y, glm::dot(n, l) + d*L.w - n.z*l.z, -n.z*L.w);
	vec4 c3(-d * l.x, -d*l.y, -d*l.z, glm::dot(n, l));
	mat4 mat(c0, c1, c2, c3);
	return mat;
}

void MyScene::STInit()
{
	//���ɲ���framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//�������Texture��Ϊ��Ⱦ����
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);	//��Ϊ�����ֵ���Ը�ʽΪ������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);		//��texture�����ӵ�0����ȸ�������

	glDrawBuffer(GL_NONE);				//�ر���ɫ�Ļ���

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;


	glBindFramebuffer(GL_FRAMEBUFFER, 0);		//���

}

void MyScene::STConstructProj(vec3 Pos)
{
	MyCamera lightCam;
	lightCam.Init(Pos, vec3(0, 0, 0));

	lightCam.SetOrtho(-10, 10, -10, 10, -20, 20);
	lightCam.SetView();

	depthMVP = lightCam.pro*lightCam.view;

	/*mat4 bias(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);*/

	//������ͼʹֵ��0~1֮��
	mat4 bias = translate(mat4(1.0), vec3(0.5, 0.5, 0.5));
	bias = scale(bias, vec3(0.5, 0.5, 0.5));

	/*vec4 test(-5,0,5,1);
	vec4 temp1 = bias*test;*/

	/*glm::mat4 m = translate(mat4(1.0),glm::vec3(0.5f, 0.5f, 0.5f))
		* scale(mat4(1.0), glm::vec3(0.5f, 0.5f, 0.5f));*/

	depthBiasMVP = bias*depthMVP;

	/*vec4 temp = depthBiasMVP*test;

	SetUniform("testDepthMVP", depthMVP, p1);*/

	//depthMVP = camera.pro*camera.view;
}

void MyScene::NormalShadowMap()
{
	glUseProgram(pShadowTex.p);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glFrontFace(GL_CW);

	/*glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(2.5f, 10.0f);*/

	DrawDepthTexture(Grid, pShadowTex);
	DrawDepthTexture(Box, pShadowTex);
	DrawDepthTexture(Sphere, pShadowTex);
	DrawDepthTexture(Neptune, pShadowTex);

	glFrontFace(GL_CCW);
	//glDisable(GL_POLYGON_OFFSET_FILL);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

/*void MyScene::ShadowTextureProcess(vec3 Pos)
{
}*/


