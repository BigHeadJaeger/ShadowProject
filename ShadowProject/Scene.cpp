#include "Scene.h"

void MyScene::Init()
{
	//着色器初始化
	glewInit();
	p1.SetShader("shader.v", "shader.f");

	pShadowTex.SetShader("shadowTex.v", "shadowTex.f");

	//相机信息初始化

	camera.Init(vec3(0, 2, 8), vec3(0, 0, 0));

	//光照初始化
	light1.InitLight(vec4(0.6f, 0.6f, 0.6f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(1, -1.0, 1, 0), vec3(1.5, 1.5, 1.5));

	

	//物体初始化
	Box.InitDirectBox(0.2, 1, 1);					//顶点、索引信息初始化
	Box.InitBuffers();							//缓冲初始化
	Box.CoorDataInit(vec3(-2.0, 1.0, 0.0),vec3(1.0,3.0,8.0));
	Box.TextureUseInfo(true, false);				//纹理使用信息初始化
	Box.InitTexture(Box.TAlbedo, "Material\\rusty\\rustediron2_basecolor.png");		//生成需要的纹理
	Box.LightUseInfo(true);						//设置是否接收光照
	Box.InitMaterial(vec4(0.5f, 0.5f, 0.5f, 1.0f), vec4(0.5, 0.5, 0.5, 1.0f), vec4(0.7f, 0.7f, 0.7f, 1.0f), 7.0f);

	/*Sphere.InitDirectSphere(1, 60, 60);
	Sphere.InitBuffers();
	Sphere.InitTexture(Sphere.AlbedoT, "Material\\oakfloor\\oakfloor_basecolor.png");*/

	Sphere.InitDirectSphere(1,20,20);					//顶点、索引信息初始化
	Sphere.InitBuffers();							//缓冲初始化
	Sphere.CoorDataInit(vec3(0.0, 1.5, 0.0));
	Sphere.TextureUseInfo(true, false);				//纹理使用信息初始化
	Sphere.InitTexture(Sphere.TAlbedo, "Material\\rusty\\rustediron2_basecolor.png");		//生成需要的纹理
	Sphere.LightUseInfo(true);						//设置是否接收光照
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
	


	STInit();		//初始化阴影贴图


}

void MyScene::Update()					//对各种矩阵进行初始化或者更新(实现动画效果)
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


	STConstructProj(-vec3(light1.Direction.x, light1.Direction.y, light1.Direction.z));		//构造阴影贴图投影矩阵
	//STConstructProj(camera.eyePos);		//构造阴影贴图投影矩阵

}

void MyScene::Draw()
{
	//将场景渲染至纹理
	/*if (testJudge)
	{
		ShadowTextureProcess();			//启用阴影贴图，进行初始化
		testJudge = false;
	}*/

	NormalShadowMap();


	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);					//三维物体要开启背面剔除
	glUseProgram(p1.p);						//启用着色器程序
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
	glEnable(GL_CULL_FACE);					//三维物体要开启背面剔除
	//glUseProgram(p1.p);						//启用着色器程序
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
										
	glBindVertexArray(obj.VAO);				//绑定前面设置好的VAO
											//传递坐标变换矩阵
	SetUniform("worldViewProj", obj.WorldViewProj, p);
	SetUniform("world", obj.World, p);
	SetUniform("worldInvTranspose", obj.WorldInvTranspose, p);
	


	//根据参数上对纹理的选择，将需要的纹理传入着色器
	//先将是否使用纹理传入shader
	SetUniform("useTexture", obj.BuseTexture, p1);
	//tempJudge = glGetUniformLocation(p.p, "useTexture");
	//glUniform1i(tempJudge, obj.BuseTexture);
	if (obj.BuseTexture)
	{
		//当物体使用纹理且有纹理坐标
		if (obj.BisTexture)
		{
			//基础反射贴图
			SetUniform("useAlbedo", obj.BAlbedo, p1);
			//tempJudge = glGetUniformLocation(p.p, "useAlbedo");
			//glUniform1i(tempJudge, obj.BAlbedo);
			if (obj.BAlbedo)
			{
				SetTexture(obj.TAlbedo, 0, GL_TEXTURE0, "albedoMap", p);

			}

		}

		//阴影贴图（与其它的纹理不是同一种类型，不需要物体自身的纹理坐标）
		SetUniform("useShadowTex", obj.BShadowTex, p1);
		if (obj.BShadowTex)
		{
			SetTexture(obj.TShadowTex, 1, GL_TEXTURE1, "shadowTex", p1);
		}
	}

	//将物体是否接收光照传入shader
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

	glBindVertexArray(obj.VAO);				//绑定前面设置好的VAO

	SetUniform("world", obj.World, p);
	SetUniform("depthMVP", depthMVP, p);

	if (obj.indexType == ObjIndexType::Triangles)
		glDrawElements(GL_TRIANGLES, obj.renderIndex.size(), GL_UNSIGNED_INT, 0);
	else if (obj.indexType == ObjIndexType::Quad)
		glDrawElements(GL_QUADS, obj.renderIndex.size(), GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void MyScene::SetTexture(GLuint & texId,int num,GLenum texNum,string samplerName,ShaderProgram&p)//参数纹理对象、采样器对应的纹理单元、要激活的纹理单元、采样器的名字、shader程序
{
	GLuint texLocation;
	glActiveTexture(texNum);							//激活纹理单元(纹理位置)0。
	glBindTexture(GL_TEXTURE_2D, texId);				//将纹理对象绑定到当前激活的纹理单元处
	//接下来指定采样器对应哪个纹理单元
	texLocation = glGetUniformLocation(p.p, samplerName.c_str());	//获取采样器的location
	glUniform1i(texLocation, num);									//指定采样器对应当前绑定的纹理单元0

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
	SetUniform("isPlaneShadow", false, p1);				//恢复为绘制物体而不是阴影
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
	//生成并绑定framebuffer
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);

	//生成深度Texture作为渲染对象
	glGenTextures(1, &depthTex);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);	//因为是深度值所以格式为深度组件
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);		//将texture与连接点0用深度附件相连

	glDrawBuffer(GL_NONE);				//关闭颜色的绘制

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;


	glBindFramebuffer(GL_FRAMEBUFFER, 0);		//解绑

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

	//修正贴图使值在0~1之间
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


