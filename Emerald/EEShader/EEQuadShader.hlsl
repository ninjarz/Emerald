#ifndef _EE_QUADSHADER_HLSL_
#define _EE_QUADSHADER_HLSL_

#include "EEObjectBuffer.hlsl"

Texture2D g_tex : register(ps, t0);

SamplerState samTex
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
	float4	posH:	SV_POSITION;
	float2	tex:	TEXCOORD;
};

QuadVOut QuadVS(QuadVIn _vIn)
{
	QuadVOut vOut;
	//transformation for the vertices of quad is not necessary
	vOut.posH = float4(_vIn.pos, 1.0f);
	vOut.tex = _vIn.tex;
	return vOut;
}

[earlydepthstencil]
void QuadPS(QuadVOut _pIn, out float4 _finalColor :SV_TARGET)
{
	_finalColor = g_tex.Sample(samTex, _pIn.tex);
	_finalColor.w = _finalColor.w * cb_alpha;
}

#endif