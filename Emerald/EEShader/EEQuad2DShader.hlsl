#ifndef _EE_QUAD2DSHADER_HLSL_
#define _EE_QUAD2DSHADER_HLSL_

#include "EEObjectBuffer.hlsl"
#include "EECameraBuffer.hlsl"

cbuffer Quad2DBuffer  : register(b3)
{
	int cb_isUseColor : packoffset(c0.x);
	int cb_isUseTex : packoffset(c0.y);
	float cb_tmp32 : packoffset(c0.z);
	float cb_tmp33 : packoffset(c0.w);
};
Texture2D g_tex0 : register(ps, t0);

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
	vOut.pos = mul(float4(_vIn.pos, 1.0f), cb_rotation);
	vOut.pos = mul(vOut.pos, cb_orthoLH);
	vOut.tex = _vIn.tex;
	return vOut;
}

[earlydepthstencil]
void QuadPS(QuadVOut _pIn, out float4 _finalColor :SV_TARGET)
{
	if (cb_isUseColor && cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, _pIn.tex) * cb_color;
	else if (cb_isUseColor)
		_finalColor = cb_color;
	else if (cb_isUseTex)
		_finalColor = g_tex0.Sample(texSampler, _pIn.tex);
	else
		_finalColor = cb_color;

	_finalColor.w = _finalColor.w * cb_alpha;
}

#endif