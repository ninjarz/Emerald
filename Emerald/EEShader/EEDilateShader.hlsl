#ifndef _EE_DILATESHADER_HLSL_
#define _EE_DILATESHADER_HLSL_

#define GROUPDIM_X 32
#define GROUPDIM_Y 32
#define GROUPDIM_Z 1

// Dilate
//----------------------------------------------------------------------------------------------------
cbuffer DilateBuffer : register(b3)
{
	int2 cb_kernel : packoffset(c0.x);
	float cb_tmp32 : packoffset(c0.z);
	float cb_tmp33 : packoffset(c0.w);
};

Texture2D<float4> g_input : register(cs, t0);
Texture2D<float> g_input_gray : register(cs, t1);
RWTexture2D<float4> g_output : register(cs, u0);

[numthreads(GROUPDIM_X, GROUPDIM_Y, GROUPDIM_Z)]
void DilateCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	uint2 index = _threadID.xy;

		int i = max((int)_threadID.x - cb_kernel.x, 0);
	int ie = min(_threadID.x + cb_kernel.x, g_input.Length.x - 1);
	for (; i <= ie; ++i)
	{
		int j = max((int)_threadID.y - cb_kernel.y, 0);
		int je = min(_threadID.y + cb_kernel.y, g_input.Length.y - 1);
		for (; j <= je; ++j)
		{
			if (g_input_gray[uint2(i, j)] > g_input_gray[index])
				index = uint2(i, j);
		}
	}

	g_output[_threadID.xy] = g_input[index];
}

#endif