#ifndef _EE_POINTS2D_SHADER_H_
#define _EE_POINTS2D_SHADER_H_

#include "EEObjectBuffer.hlsl"
#include "EECameraBuffer.hlsl"

struct PointsVIn
{
	float3	pos:	POSITION;
};

struct PointsVOut
{
	float4	pos:	SV_POSITION;
};

PointsVOut PointsVS(PointsVIn _vIn)
{
	PointsVOut vOut;
	vOut.pos = mul(float4(_vIn.pos, 1.0f), cb_worldViewProjMatrix);
	return vOut;
}

[earlydepthstencil]
void PointsPS(PointsVOut _pIn, out float4 _finalColor :SV_TARGET)
{
	_finalColor = cb_color;

	_finalColor.w = _finalColor.w * cb_alpha;
}



#endif