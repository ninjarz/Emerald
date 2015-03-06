#include "EEHelper.h"

#include <Windows.h>

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	bool EEIsBigEndian()
	{
		unsigned short test = 0x1234;

		if (*((unsigned char*)&test) == 0x12)
			return true;
		else
			return false;
	}

	//----------------------------------------------------------------------------------------------------
	void ResumeSpeed()
	{
		WORD* byte_pos = (WORD*)::timeGetTime;
		if (*byte_pos == 0x3D83) // cmp [83,3D] ЦёБо  
			return;

		HANDLE hMyHandle = ::OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ::GetCurrentProcessId());
		if (hMyHandle)
		{
			BYTE cmp_[7] = { 0x83, 0x3D, 0x14, 0x00, 0xB3, 0x76, 0x00 };
			::WriteProcessMemory(hMyHandle, (BYTE*)byte_pos, cmp_, 7, 0);
			::CloseHandle(hMyHandle);
		}
	}
}