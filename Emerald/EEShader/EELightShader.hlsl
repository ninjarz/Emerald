#ifndef _EE_LIGHTSHADER_HLSL_
#define _EE_LIGHTSHADER_HLSL_

// 平行光
struct DirLight
{
	float4	ambient;	//环境光
	float4	diffuse;	//漫反射光
	float4	specular;	//高光

	float3	dir;		//方向
	float	unused;		//“4D向量”对齐用
};

// 点光源
struct PointLight
{
	float4	ambient;	//环境光
	float4	diffuse;	//漫反射光
	float4	specular;	//高光

	float3	pos;		//光源位置
	float	range;		//光源照射范围

	float3	att;		//光强衰减系数
	float	unused;		//"4D向量"对齐用
};

// 聚光灯
struct SpotLight
{
	float4	ambient;	//环境光
	float4	diffuse;	//漫反射光
	float4	specular;	//高光

	float3	dir;		//方向
	float	range;		//照射范围

	float3	pos;		//位置
	float	spot;		//聚光强度系数

	float3	att;		//误差系数
	float	theta;		//最大发散角度
};

// 材质
struct Material
{
	float4	ambient;
	float4	diffuse;
	float4	specular;	//specular中第4个元素代表材质的表面光滑程度
};

// 平行光光照计算
void ComputeDirLight(Material mat,			//材质
	DirLight dirLight,		//平行光
	float3 normal,			//顶点法线
	float3 toEye,			//"顶点->眼"向量
	out float4 ambient,		//计算结果：环境光部分
	out float4 diffuse,		//计算结果：漫反射部分
	out float4 specular)	//计算结果：高光部分
{
	//结果首先清零
	ambient = float4(0.f, 0.0f, 0.f, 0.f);
	diffuse = float4(0.f, 0.f, 0.f, 0.f);
	specular = float4(0.f, 0.f, 0.f, 0.f);

	//环境光直接计算
	ambient = mat.ambient * dirLight.ambient;

	//计算漫反射系数
	//注意：计算前保证法线、光线方向归一化
	float diffFactor = -dot(normal, dirLight.dir);

	//如果系数小于0（即顶点背着光源），则不再进行计算
	[flatten]
	if (diffFactor > 0)
	{
		//计算漫反射光
		diffuse = mat.diffuse * dirLight.diffuse * diffFactor;

		float3 refLight = reflect(dirLight.dir, normal);
			float specFactor = pow(max(dot(refLight, toEye), 0.f), mat.specular.w);
		specular = mat.specular * dirLight.specular * specFactor;
	}
}

// 点光源光照计算
void ComputePointLight(Material mat,			//材质
	PointLight pLight,		//点光源
	float3 normal,			//法线
	float3 position,		//顶点位置
	float3 toEye,			//"顶点->眼"向量
	out float4 ambient,		//计算结果：环境光部分
	out float4 diffuse,		//计算结果：漫反射部分
	out float4 specular)	//计算结果：高光部分
{
	//结果首先清零
	ambient = float4(0.f, 0.f, 0.f, 0.f);
	diffuse = float4(0.f, 0.f, 0.f, 0.f);
	specular = float4(0.f, 0.f, 0.f, 0.f);

	//计算光照方向：顶点->光源
	float3 dir = pLight.pos - position;

		//计算顶点到光源距离
		float dist = length(dir);

	//超过照射范围，则不再进行计算
	if (dist > pLight.range)
		return;

	//归一化光线方向
	dir /= dist;

	//计算光强的衰减
	float att = 1 / (pLight.att.x + pLight.att.y*dist + pLight.att.z*dist*dist);

	//计算环境光
	ambient = mat.ambient * pLight.ambient * att;

	//计算漫反射系数
	float diffFactor = dot(dir, normal);

	//如果小于0，直接退出
	if (diffFactor > 0)
	{
		//计算漫反射光
		diffuse = mat.diffuse * pLight.diffuse * diffFactor * att;

		float3 refLight = reflect(-dir, normal);
			//计算高光系数
			float specFactor = pow(max(dot(refLight, toEye), 0.f), mat.specular.w);

		//计算高光
		specular = mat.specular * pLight.specular * specFactor * att;
	}
}

// 聚光灯光照计算
void ComputeSpotLight(Material mat,			//材质
	SpotLight L,			//聚光灯
	float3 normal,			//法线
	float3 position,		//顶点位置
	float3 toEye,			//"顶点->眼"向量
	out float4 ambient,		//计算结果：环境光部分
	out float4 diffuse,		//计算结果：漫反射部分
	out float4 specular)	//计算结果：高光部分
{
	//结果首先清零
	ambient = float4(0.f, 0.f, 0.f, 0.f);
	diffuse = float4(0.f, 0.f, 0.f, 0.f);
	specular = float4(0.f, 0.f, 0.f, 0.f);

	//计算光照方向：顶点->光源
	float3 dir = L.pos - position;
		//计算顶点到光源距离
		float dist = length(dir);

	//如果距离大于光照范围，则不再进行计算
	if (dist > L.range)
		return;

	//归一化光线方向
	dir /= dist;

	//计算衰减系数
	float att = 1 / (L.att.x + L.att.y*dist + L.att.z*dist*dist);

	//计算聚光衰减系数
	float tmp = -dot(dir, L.dir);
	if (tmp < cos(L.theta))
		return;
	float spotFactor = pow(max(tmp, 0.f), L.spot);

	//计算环境光
	ambient = mat.ambient * L.ambient * att * spotFactor;

	//计算漫反射系数
	float diffFactor = dot(dir, normal);

	//如果小于0，直接退出
	if (diffFactor > 0)
	{
		//计算漫反射光
		diffuse = mat.diffuse * L.diffuse * diffFactor * att * spotFactor;

		float3 refLight = reflect(-dir, normal);
			//计算高光系数
			float specFactor = pow(max(dot(refLight, toEye), 0.f), mat.specular.w);

		//计算高光
		specular = mat.specular * L.specular * specFactor * att * spotFactor;
	}
}

#endif