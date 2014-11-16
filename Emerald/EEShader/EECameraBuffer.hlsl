#ifndef _EE_CAMERABUFFER_HLSL_
#define _EE_CAMERABUFFER_HLSL_

cbuffer CameraBuffer : register(b1)
{
	row_major matrix  cb_orthoLH : packoffset(c0);
};

#endif