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
	if (cb_isUseColor && cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, cb_texIndex)) * cb_color;
	else if (cb_isUseColor)
		_finalColor = cb_color;
	else if (cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, float3(_pIn.tex, cb_texIndex));
	else
		_finalColor = cb_color;

	/*
	if (_pIn.pos.y < 320)
	{
		if (_finalColor.x + _finalColor.y + _finalColor.z > 1.7f)
			_finalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
		else
			_finalColor = float4(1.0f * _pIn.pos.x / 300, 1.0f * _pIn.pos.y / 500, 0.0f, 1.0f);
	}
	else if (_finalColor.x + _finalColor.y + _finalColor.z > 0.8f)
		_finalColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	else
		_finalColor = float4(1.0f * _pIn.pos.x / 300, 1.0f * _pIn.pos.y / 500, 0.0f, 1.0f);
		*/

	_finalColor.w = _finalColor.w * cb_alpha;
}

#endif