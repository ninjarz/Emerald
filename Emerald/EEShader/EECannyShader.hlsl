#ifndef _EE_ERODESHADER_HLSL_
#define _EE_ERODESHADER_HLSL_

#define GROUPDIM_X 32
#define GROUPDIM_Y 32
#define GROUPDIM_Z 1

// Canny
//----------------------------------------------------------------------------------------------------
cbuffer CannyBuffer : register(b3)
{
	int2 cb_kernel : packoffset(c0.x);
	float cb_tmp32 : packoffset(c0.z);
	float cb_tmp33 : packoffset(c0.w);
};

Texture2D<float4> g_input : register(cs, t0);
Texture2D<float> g_input_gray : register(cs, t1);
RWTexture2D<float4> g_output : register(cs, u0);

[numthreads(GROUPDIM_X, GROUPDIM_Y, GROUPDIM_Z)]
void CannyCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	float4 values = {
		g_input_gray[uint2(_threadID.x, _threadID.y)],
		g_input_gray[uint2(_threadID.x + 1, _threadID.y)],
		g_input_gray[uint2(_threadID.x, _threadID.y + 1)],
		g_input_gray[uint2(_threadID.x + 1, _threadID.y + 1)],
	};
	float p = (values[1] + values[3] - values[0] - values[2]) / 2;
	float q = (values[0] + values[1] - values[2] - values[3]) / 2;
	float m = sqrt(pow(p, 2) + pow(q, 2));

	if (m > 0.5f)
		g_output[_threadID.xy] = 1.f;
	else
		g_output[_threadID.xy] = 0.f;
}

#endif