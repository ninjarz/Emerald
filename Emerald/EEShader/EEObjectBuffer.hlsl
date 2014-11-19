#ifndef _EE_OBJECTBUFFER_HLSL_
#define _EE_OBJECTBUFFER_HLSL_

cbuffer ObjectBuffer : register(b0)
{
	row_major matrix  cb_rotation : packoffset(c0);
	float4 cb_color : packoffset(c4);
	float cb_alpha : packoffset(c5.x);
	float cb_tmp1 : packoffset(c5.y);
	float cb_tmp2 : packoffset(c5.z);
	float cb_tmp3 : packoffset(c5.w);
};

#endif