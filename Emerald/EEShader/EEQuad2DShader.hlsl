#ifndef _EE_QUAD2DSHADER_HLSL_
#define _EE_QUAD2DSHADER_HLSL_

#include "EEObjectBuffer.hlsl"
#include "EECameraBuffer.hlsl"

Texture2DArray<float4> g_tex0 : register(ps, t0);

SamplerState texSampler
{
	Filter = ANISOTROPIC;
};

struct QuadVIn
{
	float3	pos:	POSITION;
	float2	tex:	TEXCOORD;
};

struct QuadVOut
{
	float4	pos:	SV_POSITION;
	float2	tex:	TEXCOORD;
};

QuadVOut QuadVS(QuadVIn _vIn)
{
	QuadVOut vOut;
	vOut.pos = mul(float4(_vIn.pos, 1.0f), cb_worldViewProjMatrix);
	vOut.tex = _vIn.tex;
	return vOut;
}

[earlydepthstencil]
void QuadPS(QuadVOut _pIn, out float4 _finalColor :SV_TARGET)
{
	//uint width, height;
	//g_tex0.GetDimensions(width, height);
	//g_tex0.Length.x

	if (cb_isUseColor && cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, cb_texIndex)) * cb_color;
		//_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, 15)) * cb_color;
		//_finalColor = g_tex0[Float2ToUint2(_pIn.tex, g_tex0.Length.x, g_tex0.Length.y)] * cb_color;
		//_finalColor = UintToFloat4(g_tex0[Float2ToUint2(_pIn.tex, g_tex0.Length.x, g_tex0.Length.y)]) * cb_color;
		//_finalColor = UintToFloat4(Float4ToUint(g_tex0[Float2ToUint2(_pIn.tex, g_tex0.Length.x, g_tex0.Length.y)])) * cb_color;
	else if (cb_isUseColor)
		_finalColor = cb_color;
	else if (cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, cb_texIndex));
		//_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, 15));
		//_finalColor = g_tex0[Float2ToUint2(_pIn.tex, g_tex0.Length.x, g_tex0.Length.y)];
		//_finalColor = UintToFloat4(g_tex0[Float2ToUint2(_pIn.tex, g_tex0.Length.x, g_tex0.Length.y)]);
		//_finalColor = UintToFloat4(Float4ToUint(g_tex0[Float2ToUint2(_pIn.tex, g_tex0.Length.x, g_tex0.Length.y)]));
	else
		_finalColor = cb_color;

	_finalColor.w = _finalColor.w * cb_alpha;
}

#endif