
#version 430 

struct DirectionalLight
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 direction;
	vec3 intensity;
};

struct Material
{
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	float shiness;
};


//顶点着色器传入参数
in vec3 posW;
in vec3 normalW;
in vec2 TexCoord;
in vec4 shadowCoord;

//外界传入参数
//各种纹理采样器
uniform sampler2D albedoMap;
uniform sampler2DShadow shadowTex;
//其它参数
uniform bool useTexture;
uniform bool useAlbedo;
uniform bool useMetallic;
uniform bool useRoughness;
uniform bool useAo;
uniform bool useNormal;
uniform bool useShadowTex;

//光照信息(此处使用的是点光源)
uniform bool useLight=false;
uniform DirectionalLight dirLight;
uniform Material material;
//眼睛位置
uniform vec3 eyePos;

out vec4 FragColor;

uniform bool isPlaneShadow=false;				//判断渲染物体是阴影



void ComputeDirectionalLight(Material mat,
	DirectionalLight L,
	vec3 normal,
	vec3 toEye,
	out vec4 ambient,
	out vec4 diffuse,
	out vec4 specular)
{
	ambient = vec4(0.f, 0.f, 0.f, 0.f);
	diffuse = vec4(0.f, 0.f, 0.f, 0.f);
	specular = vec4(0.f, 0.f, 0.f, 0.f);

	vec3 LightV = -L.direction.xyz;
	LightV=normalize(LightV);
	//vec3 LightV = normalize(L.position);

	ambient = L.ambient*mat.ambient;

	float diffuseFactor = dot(LightV, normal);

	if (diffuseFactor>0.0f)
	{
		vec3 v = reflect(-LightV, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0f), mat.shiness);

		diffuse = diffuseFactor*L.diffuse*mat.diffuse;
		specular = specFactor*L.specular*mat.specular;
	}
}



void main() 
{
	vec3 color=vec3(0.0,0.0,0.0);
	if(!isPlaneShadow)
	{
		vec3 texColor=vec3(1.f,1.f,1.f);
		//从各种贴图中获取数据
		if(useTexture)
		{
			if(useAlbedo)
			{
				//vec3 albedo=pow(texture(albedoMap,TexCoord).rgb,vec3(2.2));			//反射率纹理一般创建在rgb空间，所以需要转换到线性空间
				texColor=texture(albedoMap,TexCoord).rgb;
				color=texColor;
			}
		}

		//执行光照
		if(useLight)
		{
			//如果当前点的深度大于此点在阴影贴图中的深度，说明这个点在阴影中
			float bias=0.000;
			float visibility=1.0;
			if(useShadowTex)
			{
				//if(texture(shadowTex,shadowCoord.xy).z+bias<shadowCoord.z)
				if(texture(shadowTex,vec3(shadowCoord.xy,shadowCoord.z-bias))!=1)
				visibility=0.5;	
			}
				
			vec3 toEye=normalize(eyePos-posW);
			vec3 normal = normalize(normalW);

			vec4 A = vec4(0.f, 0.f, 0.f, 0.f);
			vec4 D = vec4(0.f, 0.f, 0.f, 0.f);
			vec4 S = vec4(0.f, 0.f, 0.f, 0.f);

			vec4 ambient,diffuse,specular;
			ComputeDirectionalLight(material,dirLight,normal,toEye,ambient,diffuse,specular);
			A += ambient;
			D += diffuse;
			S += specular;
			//纹理加光照计算公式 纹理*（环境+漫反射）+高光
			color = (vec4(texColor,1.0)*(A + D*visibility) + S*visibility).rgb*dirLight.intensity;
		}
	}


    FragColor = vec4(color, 1.0); 
} 