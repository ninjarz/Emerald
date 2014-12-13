#include "EEHelper.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	bool EEIsBigEndian()
	{
		unsigned short test = 0x1234;

		if (*((unsigned char*)&test) == 0x12)
			return true;
		else
			return false;
	}
}