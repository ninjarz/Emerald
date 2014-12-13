#pragma once
#ifndef _EE_HELPER_H_
#define _EE_HELPER_H_

#define SAFE_RELEASE(x)      { if(x) { (x)->Release(); (x)=NULL; } }
#define SAFE_DELETE(x)       { if(x) { delete (x);     (x)=NULL; } }
#define SAFE_DELETE_ARRAY(x) { if(x) { delete[] (x);   (x)=NULL; } }

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	bool EEIsBigEndian();
}

#endif

//D3D11_USAGE_
//----------------------------------------------------------------------------------------------------
//	Resource_Usage	Default	Dynamic	Immutable	Staging
//	GPU - Read		yes		yes		yes			yes1
//	GPU - Write		yes							yes1
//	CPU - Read									yes1
//	CPU - Write				yes					yes1