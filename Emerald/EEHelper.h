#pragma once
#ifndef _EE_HELPER_H_
#define _EE_HELPER_H_

#include <string>

#define SAFE_RELEASE(x)      { if(x) { (x)->Release(); (x)=nullptr; } }
#define SAFE_DELETE(x)       { if(x) { delete (x);     (x)=nullptr; } }
#define SAFE_DELETE_ARRAY(x) { if(x) { delete[] (x);   (x)=nullptr; } }

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	bool EEIsBigEndian();
	void ResumeSpeed();
	std::string EEOpenFile();
	std::wstring AnsiToUnicode(const char* _str);
	std::wstring AnsiToUnicode(const std::string& _str);
	std::string UnicodeToAnsi(const wchar_t* _str);
	std::string UnicodeToAnsi(const std::wstring& _str);

	std::wstring CopyEnvironmentVariable(const std::wstring& variable);

	void TXTFilter(const std::string& _fin, const std::string& _fout);
	std::string& trim(std::string& _str);

	unsigned int RotateL(const unsigned int& value, const unsigned int& shift);
	unsigned long long int RotateL(const unsigned long long int& value, const unsigned long long int& shift);
	unsigned int RotateR(const unsigned int& value, const unsigned int& shift);
	unsigned long long int RotateR(const unsigned long long int& value, const unsigned long long int& shift);
	unsigned int ShiftR(const unsigned int& value, const unsigned int& shift);
	unsigned long long int ShiftR(const unsigned long long int& value, const unsigned long long int& shift);
}

#endif

// D3D11_USAGE_
//----------------------------------------------------------------------------------------------------
//	Resource_Usage	Default	Dynamic	Immutable	Staging
//	GPU - Read		yes		yes		yes			yes1
//	GPU - Write		yes							yes1
//	CPU - Read									yes1
//	CPU - Write				yes					yes1


// 8位浮点数
//----------------------------------------------------------------------------------------------------
// 0000 0000(2) = 0.0f
// 0000 0001(2) = 1/255.0f
// 1111 1111(2) = 1.0f
// ...


// Shader
//----------------------------------------------------------------------------------------------------
// texture, Texture1D, Texture1DArray, Texture2D, Texture2DArray, Texture3D, TextureCube. The element size must fit into 4 32-bit quantities.


// Compute Shader
//----------------------------------------------------------------------------------------------------
// SV_GroupIndex, SV_DispatchThreadID, SV_GroupThreadID, SV_GroupID
// Dispatch(x, y, z)
// numthreads(a, b, c) ; In cs_5_0, the max of c is 64 and the max of a * b * c is 1024
// SV_GroupID : Refer to Dispatch box[x'][y'][z']
// SV_GroupThreadID : Inside the SV_GroupID, refer to Group box[a'][b'][c']
// SV_GroupIndex : Inside the SV_GroupID, Refer to a' + b' * a + c' * b * a
// SV_DispatchThreadID : Global index, Refer to SV_GroupID * numthreads + SV_GroupThreadID
//
// NOTE, out of bound reads always return 0 in CS


// Map
//----------------------------------------------------------------------------------------------------
// RowPitch and DepthPitch that mapped from resource are not match its size.
//
//


// Gouraud Shading, Phong Shading
//----------------------------------------------------------------------------------------------------
// Gouraud: 亮度插值，会产生Mach带效应
// Phong: 法向量插值，计算量大


// Whitted
//----------------------------------------------------------------------------------------------------
// I=Ic+KsIs+KtIt (s, t方向迭代)


// YUV And RGB
//----------------------------------------------------------------------------------------------------
// Y = 0.299 * R + 0.587 * G + 0.114 * B
// U = -0.147 * R - 0.289 * G + 0.436 * B = 0.492 * (B - Y)
// V = 0.615 * R - 0.515 * G - 0.100 * B = 0.877 * (R - Y)
//
// R = Y + 1.140 * V
// G = Y - 0.394 * U - 0.581 * V
// B = Y + 2.032 * U


// YCbCr And RGB
//----------------------------------------------------------------------------------------------------
// Y = 0.257 * R + 0.504 * G + 0.098 * B + 16
// Cb = -0.148 * R - 0.291 * G + 0.439 * B + 128
// Cr = 0.439 * R - 0.368 * G - 0.071 * B + 128
//
// R = 1.164 * (Y - 16) + 1.596 * (Cr - 128)
// G = 1.164 * (Y - 16) - 0.813 * (Cr - 128) - 0.392 * (Cb - 128)
// B = 1.164 * (Y - 16) + 2.017 * (Cb - 128)


// YV12
//----------------------------------------------------------------------------------------------------
// YV12 (4 : 2 : 0)
// Y(width * heigth) ＋ V(width * heigth / 4) + U(width * heigth / 4)
// size: width * heigth * 1.5 Byte


// I420
//----------------------------------------------------------------------------------------------------
// I420 (4 : 2 : 0)
// Y(width * heigth) ＋ U(width * heigth / 4) + V(width * heigth / 4)
// size: width * heigth * 1.5 Byte