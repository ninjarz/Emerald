#include "DIVAHelper.h"
#if _DIVA_

#include "DIVAConfig.h"


bool DIVAConfig::s_isInitialized = false;
std::map<unsigned int, char> DIVAConfig::s_keyMap;

bool DIVAConfig::Initialize()
{
	if (!s_isInitialized)
	{
		s_keyMap[0] = 'D';
		s_keyMap[1] = 'A';
		s_keyMap[2] = 'S';
		s_keyMap[3] = 'W';

		s_keyMap[4] = 'd';
		s_keyMap[5] = 'd';
		s_keyMap[6] = 'd';
		s_keyMap[7] = 'd';

		s_keyMap[8] = 'D';
		s_keyMap[9] = 'A';
		s_keyMap[10] = 'S';
		s_keyMap[11] = 'W';

		s_keyMap[12] = 'd';
		s_keyMap[13] = 'd';
		s_keyMap[14] = 'd';
		s_keyMap[15] = 'd';
	}

	return true;
}

char DIVAConfig::GetKeyMap(unsigned int _key)
{
	Initialize();

	auto it = s_keyMap.find(_key);
	if (it != s_keyMap.end())
		return it->second;
	else
		return (char)0;
}

#endif