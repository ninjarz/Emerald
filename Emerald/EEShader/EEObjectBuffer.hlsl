#ifndef _EE_OBJECTBUFFER_HLSL_
#define _EE_OBJECTBUFFER_HLSL_

cbuffer ObjectBuffer : register(b0)
{
	float cb_alpha : packoffset(c0.x);
	float cb_tmp1 : packoffset(c0.y);
	float cb_tmp2 : packoffset(c0.z);
	float cb_tmp3 : packoffset(c0.w);
};

#endif