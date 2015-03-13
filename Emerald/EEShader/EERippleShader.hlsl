#ifndef _EE_RIPPLESHADER_HLSL_
#define _EE_RIPPLESHADER_HLSL_

#include "EEShaderHelper.hlsl"

//Ripple
//----------------------------------------------------------------------------------------------------
Texture2D<int> g_weight : register(cs, t0);
Texture2D<float4> g_input : register(cs, t1);
RWTexture2D<float4> g_output : register(cs, u0);

[numthreads(32, 32, 1)]
void RippleCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	uint width = g_weight.Length.x;
	uint height = g_weight.Length.y;

	uint data = 1024 - g_weight[uint2(_threadID.x, _threadID.y)].x;
	int a = ((_threadID.x - width / 2.f) * data / 1024.f) + width / 2.f;
	int b = ((_threadID.y - height / 2.f) * data / 1024.f) + height / 2.f;

	/*
	if (a >= width)
		a = width - 1;
	else if (a < 0)
		a = 0;

	if (b >= height)
		b = height - 1;
	else if (b < 0)
		b = 0;
		*/
	//Set to the average color value

	// Deal with refraction
	g_output[uint2(_threadID.x, _threadID.y)] = g_input[uint2(a, b)];
}

#endif