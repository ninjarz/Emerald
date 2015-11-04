#include "DIVAConfig.h"


//----------------------------------------------------------------------------------------------------
bool DIVAConfig::s_isInitialized = false;
std::map<unsigned int, char> DIVAConfig::s_keyMap;

//----------------------------------------------------------------------------------------------------
bool DIVAConfig::Initialize()
{
	if (!s_isInitialized)
	{
		s_keyMap[0] = 'D';
		s_keyMap[1] = 'A';
		s_keyMap[2] = 'S';
		s_keyMap[3] = 'W';

		s_keyMap[4] = (char)39;
		s_keyMap[5] = (char)37;
		s_keyMap[6] = (char)40;
		s_keyMap[7] = (char)38;

		s_keyMap[8] = 'D';
		s_keyMap[9] = 'A';
		s_keyMap[10] = 'S';
		s_keyMap[11] = 'W';

		s_keyMap[12] = (char)39;
		s_keyMap[13] = (char)37;
		s_keyMap[14] = (char)40;
		s_keyMap[15] = (char)38;
	}

	return true;
}

//----------------------------------------------------------------------------------------------------
char DIVAConfig::GetKeyMap(unsigned int _key)
{
	Initialize();

	auto it = s_keyMap.find(_key);
	if (it != s_keyMap.end())
		return it->second;
	else
		return (char)0;
}