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
enum NOTE_TYPE { NOTETYPE_NORMAL = 0, NOTETYPE_STRIP = 1 };
enum NOTE_KEY { NOTE_CIRCLE = 0, NOTE_RECT = 1, NOTE_CROSS = 2, NOTE_TRIANGLE = 3, NOTE_RIGHT = 4, NOTE_LEFT = 5, NOTE_DOWN = 6, NOTE_UP = 7 };

//----------------------------------------------------------------------------------------------------
class DIVAConfig
{
public:
	static bool Initialize();
	static char GetKeyMap(unsigned int _key);

private:
	static bool s_isInitialized;
	static std::map<unsigned int, char> s_keyMap;

	static int s_framesPerBeat;
	static int s_periodPerBeat;
	static int s_notesNum;
	static int s_bgsMax;
};
