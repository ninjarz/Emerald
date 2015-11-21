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
enum DIVADifficulty
{

};

//----------------------------------------------------------------------------------------------------
class DIVAConfig
{
public:
	static bool Initialize();

	static char GetKeyMap(unsigned int _key);
	static int GetFramesPerBeat();
	static int GetPeriodsPerBeat();
	static int GetNoteNum();
	static int GetMusicMaxPerFrame();
	static int GetFramesPerBeat();
	static int GetNoteWidth();
	static int GetNoteHeight();

private:
	static bool s_isInitialized;

	static std::map<unsigned int, char> s_keyMap;
	static int s_framesPerBeat;
	static int s_periodsPerBeat;
	static int s_noteNum;
	static int s_musicMaxPerFrame;
	static int s_noteWidth;
	static int s_noteHeight;
};
