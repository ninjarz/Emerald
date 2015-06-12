#ifndef _EE_GRAYSHADER_HLSL_
#define _EE_GRAYSHADER_HLSL_

#define GROUPDIM_X 32
#define GROUPDIM_Y 32
#define GROUPDIM_Z 1

// Gray
//----------------------------------------------------------------------------------------------------
cbuffer GrayBuffer : register(b3)
{
	float cb_value : packoffset(c0.x);
	float cb_tmp31 : packoffset(c0.z);
	float cb_tmp32 : packoffset(c0.w);
	float cb_tmp33 : packoffset(c0.y);
};

Texture2D<float4> g_input : register(cs, t0);
RWTexture2D<float4> g_output : register(cs, u0);

[numthreads(GROUPDIM_X, GROUPDIM_Y, GROUPDIM_Z)]
void GrayCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	float4 value = g_input[uint2(_threadID.x, _threadID.y)];
	float gray = value.x * 0.299 + value.y * 0.587 + value.z * 0.114;

	g_output[uint2(_threadID.x, _threadID.y)] = gray;
}

#endif