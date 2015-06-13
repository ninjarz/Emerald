#ifndef _EE_GRAYSHADER_HLSL_
#define _EE_GRAYSHADER_HLSL_

#define GROUPDIM_X 32
#define GROUPDIM_Y 32
#define GROUPDIM_Z 1

// ContrastAndBright (f(x) * (c * 0.01f) + b)
//----------------------------------------------------------------------------------------------------
cbuffer ContrastAndBrightBuffer : register(b3)
{
	float cb_contrast : packoffset(c0.x);
	float cb_bright : packoffset(c0.y);
	float cb_tmp32 : packoffset(c0.z);
	float cb_tmp33 : packoffset(c0.w);
};

Texture2D<float4> g_input : register(cs, t0);
RWTexture2D<float4> g_output : register(cs, u0);

[numthreads(GROUPDIM_X, GROUPDIM_Y, GROUPDIM_Z)]
void ContrastAndBrightCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	float4 value = g_input[uint2(_threadID.x, _threadID.y)];

	g_output[uint2(_threadID.x, _threadID.y)] = value * (cb_contrast * 0.01f) + cb_bright;
}

#endif