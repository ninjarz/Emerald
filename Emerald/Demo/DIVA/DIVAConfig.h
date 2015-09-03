#pragma once

#include <map>
using namespace std;

class DIVAConfig
{
public:
	static bool Initialize();
	static char GetKeyMap(unsigned int _key);
	
private:
	static bool s_isInitialized;
	static std::map<unsigned int, char> s_keyMap;
};
