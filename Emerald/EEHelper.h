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
}

#endif

//D3D11_USAGE_
//----------------------------------------------------------------------------------------------------
//	Resource_Usage	Default	Dynamic	Immutable	Staging
//	GPU - Read		yes		yes		yes			yes1
//	GPU - Write		yes							yes1
//	CPU - Read									yes1
//	CPU - Write				yes					yes1



//8Î»¸¡µãÊý
//----------------------------------------------------------------------------------------------------
//0000 0000(2) = 0.0f
//0000 0001(2) = 1/255.0f
//1111 1111(2) = 1.0f
//...

//Shader
//----------------------------------------------------------------------------------------------------
//texture, Texture1D, Texture1DArray, Texture2D, Texture2DArray, Texture3D, TextureCube. The element size must fit into 4 32-bit quantities.

//Compute Shader
//----------------------------------------------------------------------------------------------------
//SV_GroupIndex, SV_DispatchThreadID, SV_GroupThreadID, SV_GroupID
//Dispatch(x, y, z)
//numthreads(a, b, c) ; In cs_5_0, the max of c is 64 and the max of a * b * c is 1024
//SV_GroupID : Refer to Dispatch box[x'][y'][z']
//SV_GroupThreadID : Inside the SV_GroupID, refer to Group box[a'][b'][c']
//SV_GroupIndex : Inside the SV_GroupID, Refer to a' + b' * a + c' * b * a
//SV_DispatchThreadID : Global index, Refer to SV_GroupID * numthreads + SV_GroupThreadID
//
//NOTE, out of bound reads always return 0 in CS

//Map
//----------------------------------------------------------------------------------------------------
//RowPitch and DepthPitch that mapped from resource are not match its size.
//
//