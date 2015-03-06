#pragma once
#ifndef _EE_HELPER_H_
#define _EE_HELPER_H_

#define SAFE_RELEASE(x)      { if(x) { (x)->Release(); (x)=nullptr; } }
#define SAFE_DELETE(x)       { if(x) { delete (x);     (x)=nullptr; } }
#define SAFE_DELETE_ARRAY(x) { if(x) { delete[] (x);   (x)=nullptr; } }

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	bool EEIsBigEndian();
	void ResumeSpeed();
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