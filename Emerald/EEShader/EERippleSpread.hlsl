#ifndef _EE_RIPPLESPREAD_HLSL_
#define _EE_RIPPLESPREAD_HLSL_

#include "EERippleHelper.hlsl"

//Spread
//----------------------------------------------------------------------------------------------------
cbuffer SpreadBuffer : register(b3)
{
	int cb_spreadFactor : packoffset(c0.x);
	int cb_fadeFactor : packoffset(c0.y);
	float cb_tmp31 : packoffset(c0.z);
	float cb_tmp32 : packoffset(c0.w);
};

Texture2D<int> g_input : register(cs, t0);
RWTexture2D<int> g_output : register(cs, u0);

//groupshared int s_data[GROUPDIM_X + 2][GROUPDIM_Y + 2];
[numthreads(GROUPDIM_X, GROUPDIM_Y, GROUPDIM_Z)]
void SpreadCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	//X0' = £¨(X1 + X2 + X3 + X4) >> cb_spreadFactor - X0'£© >> cb_fadeFactor
	// - X0' because of the potential energy
	g_output[uint2(_threadID.x, _threadID.y)] =
		(((g_input[uint2(_threadID.x + 1, _threadID.y)] + g_input[uint2(_threadID.x - 1, _threadID.y)] + g_input[uint2(_threadID.x, _threadID.y + 1)] + g_input[uint2(_threadID.x, _threadID.y - 1)]) 
		>> cb_spreadFactor) 
		- g_output[uint2(_threadID.x, _threadID.y)]);
	if (cb_fadeFactor < 32)
		g_output[uint2(_threadID.x, _threadID.y)] -= g_output[uint2(_threadID.x, _threadID.y)] >> cb_fadeFactor;

}

#endif