#ifndef _EE_CAMERABUFFER_HLSL_
#define _EE_CAMERABUFFER_HLSL_

#include "EEShaderHelper.hlsl"

cbuffer CameraBuffer : register(b1)
{
	row_major matrix cb_orthoLHMatrix : packoffset(c0);
	row_major matrix cb_perspectiveFovLHMatrix : packoffset(c4);
	row_major matrix cb_viewMatrix : packoffset(c8);
	float3 cb_eyePos : packoffset(c12);
	float cb_tmp0 : packoffset(c12.w);
};

#endif