#ifndef _EE_OBJECTBUFFER_HLSL_
#define _EE_OBJECTBUFFER_HLSL_

cbuffer ObjectBuffer : register(b0)
{
	row_major matrix cb_worldMatrix : packoffset(c0);
	row_major matrix cb_worldViewProjMatrix : packoffset(c4);
	row_major matrix cb_rotationMatrix : packoffset(c8);
	float4 cb_color : packoffset(c12);
	float cb_alpha : packoffset(c13.x);
	float cb_tmp01 : packoffset(c13.y);
	float cb_tmp02 : packoffset(c13.z);
	float cb_tmp03 : packoffset(c13.w);
};

#endif