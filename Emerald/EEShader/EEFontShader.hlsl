#ifndef _EE_FONTSHADER_HLSL_
#define _EE_FONTSHADER_HLSL_

#include "EEObjectBuffer.hlsl"
#include "EECameraBuffer.hlsl"

Texture2D g_tex0 : register(ps, t0);

SamplerState texSampler
{
	Filter = ANISOTROPIC;
};

struct FontVIn
{
	float3	pos:	POSITION;
	float2	tex:	TEXCOORD;
};

struct FontVOut
{
	float4	pos:	SV_POSITION;
	float2	tex:	TEXCOORD;
};

FontVOut FontVS(FontVIn _vIn)
{
	FontVOut vOut;
	vOut.pos = mul(float4(_vIn.pos, 1.0f), cb_rotation);
	vOut.pos = mul(vOut.pos, cb_orthoLH);
	vOut.tex = _vIn.tex;
	return vOut;
}

[earlydepthstencil]
void FontPS(FontVOut _pIn, out float4 _finalColor :SV_TARGET)
{
	float4 sampledColor = g_tex0.Sample(texSampler, _pIn.tex);
	if (sampledColor.r == 0.0f)
		clip(-1);
	_finalColor = sampledColor * cb_color;
	_finalColor.w = _finalColor.w * cb_alpha;
}

#endif