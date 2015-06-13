#ifndef _EE_BLURSHADER_HLSL_
#define _EE_BLURSHADER_HLSL_

#include "EEShaderHelper.hlsl"

#define N 256
#define GROUPDIM_Z 1

// Blur
//----------------------------------------------------------------------------------------------------
cbuffer BlurSettings
{
	static const int g_blurRadius = 5;
	static const float g_weights[11] =
	{
		0.05f, 0.05f, 0.1f, 0.1f, 0.1f, 0.2f, 0.1f, 0.1f, 0.1f, 0.05f, 0.05f,
	};
};

cbuffer BlurBuffer : register(b3)
{
	float cb_value : packoffset(c0.x);
	float cb_tmp31 : packoffset(c0.y);
	float cb_tmp32 : packoffset(c0.z);
	float cb_tmp33 : packoffset(c0.w);
};

Texture2D<float4> g_input : register(cs, t0);
RWTexture2D<float4> g_output : register(cs, u0);
groupshared float4 g_cache[N + 2 * g_blurRadius];

[numthreads(N, 1, GROUPDIM_Z)]
void BlurHorzCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	// left borders
	if (_groupTID.x < g_blurRadius)
	{
		// image borders
		int x = max(_threadID.x - g_blurRadius, 0);
		g_cache[_groupTID.x] = g_input[uint2(x, _threadID.y)];
	}
	// right borders
	if (_groupTID.x >= N - g_blurRadius)
	{
		// image borders.
		int x = min(_threadID.x + g_blurRadius, g_input.Length.x - 1);
		g_cache[_groupTID.x + 2 * g_blurRadius] = g_input[uint2(x, _threadID.y)];
	}
	g_cache[_groupTID.x + g_blurRadius] = g_input[min(_threadID.xy, g_input.Length.xy - 1)];

	GroupMemoryBarrierWithGroupSync();

	// blur
	float4 blurColor = float4(0.f, 0.f, 0.f, 0.f);
	[unroll]
	for (int i = -g_blurRadius; i <= g_blurRadius; ++i)
	{
		blurColor += g_weights[i + g_blurRadius] * g_cache[_groupTID.x + g_blurRadius + i];
	}
	g_output[_threadID.xy] = blurColor;
}

[numthreads(1, N, GROUPDIM_Z)]
void BlurVertCS(uint3 _groupID : SV_GroupID, uint3 _groupTID : SV_GroupThreadID, uint _groupIndex : SV_GroupIndex, uint3 _threadID : SV_DispatchThreadID)
{
	// top borders
	if (_groupTID.y < g_blurRadius)
	{
		// image borders
		int y = max(_threadID.y - g_blurRadius, 0);
		g_cache[_groupTID.y] = g_input[uint2(_threadID.x, y)];
	}
	// buttom borders
	if (_groupTID.y >= N - g_blurRadius)
	{
		// image borders.
		int y = min(_threadID.y + g_blurRadius, g_input.Length.y - 1);
		g_cache[_groupTID.y + 2 * g_blurRadius] = g_input[uint2(_threadID.x, y)];
	}
	g_cache[_groupTID.y + g_blurRadius] = g_input[min(_threadID.xy, g_input.Length.xy - 1)];

	GroupMemoryBarrierWithGroupSync();

	// blur
	float4 blurColor = float4(0.f, 0.f, 0.f, 0.f);
	[unroll]
	for (int i = -g_blurRadius; i <= g_blurRadius; ++i)
	{
		blurColor += g_weights[i + g_blurRadius] * g_cache[_groupTID.y + g_blurRadius + i];
	}
	g_output[_threadID.xy] = blurColor;
}

#endif