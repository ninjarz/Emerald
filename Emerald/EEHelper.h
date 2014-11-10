#pragma once
#ifndef _EE_HELPER_H_
#define _EE_HELPER_H_

#define SAFE_RELEASE(x)      { if(x) { (x)->Release(); (x)=NULL; } }
#define SAFE_DELETE(x)       { if(x) { delete (x);     (x)=NULL; } }
#define SAFE_DELETE_ARRAY(x) { if(x) { delete[] (x);   (x)=NULL; } }

#endif

//----------------------------------------------------------------------------------------------------