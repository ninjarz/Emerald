#include "DIVAConfig.h"
#include "../../Emerald.h"


//----------------------------------------------------------------------------------------------------
bool DIVAConfig::s_isInitialized = false;

std::map<unsigned int, char> DIVAConfig::s_keyMap;
int DIVAConfig::s_framesPerBeat = 192;
int DIVAConfig::s_periodsPerBeat = 4;
int DIVAConfig::s_noteNum = 8;
int DIVAConfig::s_musicMaxPerFrame = 10;
int DIVAConfig::s_noteWidth = 0;
int DIVAConfig::s_noteHeight = 0;

//----------------------------------------------------------------------------------------------------
bool DIVAConfig::Initialize()
{
	if (!s_isInitialized)
	{
		s_noteWidth = EEGetWidth() / 40;
		s_noteHeight = EEGetHeight() / 22;

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

		s_isInitialized = true;
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

 int DIVAConfig::GetFramesPerBeat()
{
	Initialize();

	return s_framesPerBeat;
}

 int DIVAConfig::GetPeriodsPerBeat()
{
	Initialize();

	return s_periodsPerBeat;
}

 int DIVAConfig::GetNoteNum()
{
	Initialize();

	return s_noteNum;
}

 int DIVAConfig::GetMusicMaxPerFrame()
{
	Initialize();

	return s_musicMaxPerFrame;
}

 int DIVAConfig::GetFramesPerBeat()
{
	Initialize();

	return s_framesPerBeat;
}

 int DIVAConfig::GetNoteWidth()
{
	Initialize();

	return s_noteWidth;
}

 int DIVAConfig::GetNoteHeight()
{
	Initialize();

	return s_noteHeight;
}