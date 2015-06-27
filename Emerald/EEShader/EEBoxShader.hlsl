#ifndef _EE_BOXSHADER_HLSL_
#define _EE_BOXSHADER_HLSL_

#include "EEObjectBuffer.hlsl"
#include "EECameraBuffer.hlsl"
#include "EELightShader.hlsl"

Texture2D g_tex0 : register(ps, t0);

SamplerState texSampler
{
	Filter = ANISOTROPIC;
};

struct BoxVIn
{
	float3	pos:	POSITION;
	float3	normal:	NORMAL;
	float2	tex:	TEXCOORD;
};

struct BoxVOut
{
	float4	pos:	SV_POSITION;
	float3	normal:	NORMAL;
	float2	tex:	TEXCOORD;
	float3	posW:	POSITION;
};

BoxVOut BoxVS(BoxVIn _vIn)
{
	BoxVOut vOut;
	vOut.pos = mul(float4(_vIn.pos, 1.f), cb_worldViewProjMatrix);
	vOut.normal = _vIn.normal;
	vOut.tex = _vIn.tex;
	vOut.posW = mul(float4(_vIn.pos, 1.f), cb_worldMatrix).xyz;
	return vOut;
}

[earlydepthstencil]
void BoxPS(BoxVOut _pIn, out float4 _finalColor :SV_TARGET)
{
	if (cb_isUseColor && cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, _pIn.tex) * cb_color;
	else if (cb_isUseColor)
		_finalColor = cb_color;
	//_finalColor = _pIn.pos / 100;
	else if (cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, _pIn.tex);
	else
		_finalColor = cb_color;

	_finalColor.w = _finalColor.w * cb_alpha;

	Material material;
	material.ambient = _finalColor;
	material.diffuse = _finalColor;
	material.specular = float4(_finalColor.xyz, 16.0f);
	DirLight directLight;
	directLight.ambient = float4(0.2f, 0.2f, 0.2f, 1.0f);
	directLight.diffuse = float4(0.5f, 0.5f, 0.5f, 1.0f);
	directLight.specular = float4(0.5f, 0.5f, 0.5f, 1.0f);
	directLight.dir = float3(0.67735f, -0.57735f, 0.57735f);
	float3 toEye = normalize(cb_eyePos - _pIn.posW);
	float4 A = float4(0.f, 0.f, 0.f, 0.f);
	float4 D = float4(0.f, 0.f, 0.f, 0.f);
	float4 S = float4(0.f, 0.f, 0.f, 0.f);
	ComputeDirLight(material, directLight, normalize(_pIn.normal), toEye, A, D, S);

	_finalColor = A + D + S;
}

#endif