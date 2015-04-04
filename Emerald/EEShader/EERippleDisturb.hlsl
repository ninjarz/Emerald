#ifndef _EE_RIPPLEDISTURB_HLSL_
#define _EE_RIPPLEDISTURB_HLSL_

#include "EERippleHelper.hlsl"

//Disturb
//----------------------------------------------------------------------------------------------------
cbuffer DisturbBuffer : register(b3)
{
	int2 cb_pos : packoffset(c0.x);
	int cb_range : packoffset(c0.z);
	int cb_weight : packoffset(c0.w);
};

RWTexture2D<int> g_output : register(cs, u0);

[numthreads(GROUPDIM_X, GROUPDIM_Y, GROUPDIM_Z)]
void DisturbCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	if (pow(cb_pos.x - _threadID.x, 2) + pow(cb_pos.y - _threadID.y, 2) < pow(cb_range, 2))
	{
		g_output[uint2(_threadID.x, _threadID.y)] += cb_weight;
	}
}

#endif