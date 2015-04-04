#ifndef _EE_RIPPLESHADER_HLSL_
#define _EE_RIPPLESHADER_HLSL_

#include "EERippleHelper.hlsl"

//Ripple
//----------------------------------------------------------------------------------------------------
cbuffer RippleBuffer : register(b3)
{
	float cb_refractiveIndex : packoffset(c0.x);
	float cb_tmp31 : packoffset(c0.z);
	float cb_tmp32 : packoffset(c0.w);
	float cb_tmp33 : packoffset(c0.y);
};

Texture2D<int> g_weight : register(cs, t0);
Texture2D<float4> g_input : register(cs, t1);
RWTexture2D<float4> g_output : register(cs, u0);

[numthreads(GROUPDIM_X, GROUPDIM_Y, GROUPDIM_Z)]
void RippleCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	int weight = g_weight[uint2(_threadID.x, _threadID.y)];

	if (weight)
	{
		//By self-weight only
		float halfWidth = g_weight.Length.x / 2.f;
		float halfHeight = g_weight.Length.y / 2.f;

		float factor = 1.0f - weight / cb_refractiveIndex;
		int a = ((_threadID.x - halfWidth) * factor) + halfWidth;
		int b = ((_threadID.y - halfHeight) * factor) + halfHeight;

		g_output[uint2(_threadID.x, _threadID.y)] = g_input[uint2(a, b)];
		
		//By weight diff
		//int a = g_weight[uint2(_threadID.x + 1, _threadID.y)] - g_weight[uint2(_threadID.x - 1, _threadID.y)] >> 2;
		//int b = g_weight[uint2(_threadID.x, _threadID.y + 1)] - g_weight[uint2(_threadID.x, _threadID.y - 1)] >> 2;
		//g_output[uint2(_threadID.x, _threadID.y)] = g_input[uint2(_threadID.x + a, _threadID.y + b)];



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
		//Set the value that is out of bound to the average color value

		// Deal with refraction
		//g_output[uint2(_threadID.x, _threadID.y)] = g_input[uint2(a, b)];
		//g_output[uint2(_threadID.x, _threadID.y)] = float4(0.5f, 0.5f, 0.5f, 0.5f);
	}
}

#endif