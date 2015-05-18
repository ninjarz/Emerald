#ifndef _EE_LINE2D_SHADER_H_
#define _EE_LINE2D_SHADER_H_

#include "EEObjectBuffer.hlsl"
#include "EECameraBuffer.hlsl"

Texture2DArray<float4> g_tex0 : register(ps, t0);

SamplerState texSampler
{
	Filter = ANISOTROPIC;
};

struct LineVIn
{
	float3	pos:	POSITION;
	float2	tex:	TEXCOORD;
};

struct LineVOut
{
	float4	pos:	SV_POSITION;
	float2	tex:	TEXCOORD;
};

LineVOut LineVS(LineVIn _vIn)
{
	LineVOut vOut;
	vOut.pos = mul(float4(_vIn.pos, 1.0f), cb_worldViewProjMatrix);
	vOut.tex = _vIn.tex;
	return vOut;
}

[earlydepthstencil]
void LinePS(LineVOut _pIn, out float4 _finalColor :SV_TARGET)
{
	if (cb_isUseColor && cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, cb_texIndex)) * cb_color;
	else if (cb_isUseColor)
		_finalColor = cb_color;
	else if (cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, cb_texIndex));
	else
		_finalColor = cb_color;

	_finalColor.w = _finalColor.w * cb_alpha;
}



#endif