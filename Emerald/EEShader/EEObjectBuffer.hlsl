#ifndef _EE_OBJECTBUFFER_HLSL_
#define _EE_OBJECTBUFFER_HLSL_

cbuffer ObjectBuffer : register(b0)
{
	row_major matrix  cb_rotation : packoffset(c0);
	float cb_alpha : packoffset(c4.x);
	float cb_tmp1 : packoffset(c4.y);
	float cb_tmp2 : packoffset(c4.z);
	float cb_tmp3 : packoffset(c4.w);
};

#endif