#pragma once

#include <map>
using namespace std;


//----------------------------------------------------------------------------------------------------
enum DIVAState
{
	DIVA_DEFAULT = 0,
	DIVA_MENU,
	DIVA_FREE_MODE,
	DIVA_END
};

//----------------------------------------------------------------------------------------------------
class DIVAConfig
{
public:
	static bool Initialize();
	static char GetKeyMap(unsigned int _key);

private:
	static bool s_isInitialized;
	static std::map<unsigned int, char> s_keyMap;
};
