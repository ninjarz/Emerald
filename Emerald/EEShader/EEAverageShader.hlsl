#ifndef _EE_AVERAGESHADER_HLSL_
#define _EE_AVERAGESHADER_HLSL_

// Average
//----------------------------------------------------------------------------------------------------
cbuffer AverageBuffer : register(b3)
{
	float cb_value : packoffset(c0.x);
	float cb_tmp31 : packoffset(c0.z);
	float cb_tmp32 : packoffset(c0.w);
	float cb_tmp33 : packoffset(c0.y);
};

Texture2D<float4> g_input : register(cs, t0);
RWTexture2D<float4> g_output : register(cs, u0);

[numthreads(1, 1, 1)]
void AverageCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	float4 data = g_input[uint2(0, 0)];

		
	g_output[uint2(0, 0)] = data;
}

#endif