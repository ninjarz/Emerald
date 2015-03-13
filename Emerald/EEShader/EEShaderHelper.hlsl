#ifndef _EE_SHADERHELPER_HLSL_
#define _EE_SHADERHELPER_HLSL_

//float4 -> uint
//----------------------------------------------------------------------------------------------------
uint Float4ToUint(in float4 _input)
{
	uint result = 0;
	result += _input.w * 0xff;
	result <<= 8;
	result += _input.z * 0xff;
	result <<= 8;
	result += _input.y * 0xff;
	result <<= 8;
	result += _input.x * 0xff;
	return result;
}

//uint -> float4
//----------------------------------------------------------------------------------------------------
float4 UintToFloat4(in uint _input)
{
	//_input format is A8B8G8R8 in RWTexture2D load mode, but why?
	float4 result;
	result.x = (float)(_input & 0xff) / 0xff;
	_input >>= 8;
	result.y = (float)(_input & 0xff) / 0xff;
	_input >>= 8;
	result.z = (float)(_input & 0xff) / 0xff;
	_input >>= 8;
	result.w = (float)(_input & 0xff) / 0xff;
	return result;
}

//uint -> float4
//----------------------------------------------------------------------------------------------------
void UintToFloat4(in uint _input, out float4 _result)
{
	_result.x = (float)(_input & 0xff) / 0xff;
	_input >>= 8;
	_result.y = (float)(_input & 0xff) / 0xff;
	_input >>= 8;
	_result.z = (float)(_input & 0xff) / 0xff;
	_input >>= 8;
	_result.w = (float)(_input & 0xff) / 0xff;
}

//----------------------------------------------------------------------------------------------------
uint2 Float2ToUint2(in float2 _input, in uint _width, in uint _height)
{
	return uint2(_input.x * _width, _input.y * _height);
}

//----------------------------------------------------------------------------------------------------
void Float2ToUint2(in float2 _input, in uint _width, in uint _height, out uint2 _result)
{
	_result.x = _input.x * _width;
	_result.y = _input.y * _height;
}


//----------------------------------------------------------------------------------------------------
//cbuffer
//b0	ObjectBuffer
//b1	CameraBuffer
//b2	
//b3	(share)
//		Quad2DBuffer
//		BoxBuffer


#endif