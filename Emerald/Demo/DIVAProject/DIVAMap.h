#pragma once

#include <vector>
#include <map>
#include "DIVAConfig.h"
using namespace std;


//----------------------------------------------------------------------------------------------------
enum class NoteType { NOTE_NORMAL = 0, NOTE_STRIP = 1 };
enum class NoteKey { NOTE_CIRCLE = 0, NOTE_RECT = 1, NOTE_CROSS = 2, NOTE_TRIANGLE = 3, NOTE_RIGHT = 4, NOTE_LEFT = 5, NOTE_DOWN = 6, NOTE_UP = 7 };

//----------------------------------------------------------------------------------------------------
class DIVAMap
{
	//----------------------------------------------------------------------------------------------------
	class Note
	{
	public:
		inline Note() : framePos(0), sound(0), x(0), y(0), tailX(0), tailY(0), keyType(), key() {}

	public:
		int framePos;
		int sound;
		NoteType keyType;
		NoteKey key;
		int  x, y;
		float tailX, tailY;
		float duration;
	};

	//----------------------------------------------------------------------------------------------------
	class Frame
	{
	public:
		Frame() : isAlive(true), timePos(0), beatsPerMinute(0), resource(-1) {}

		inline double GetSingleTime() { return 60.0 / (beatsPerMinute * (DIVAConfig::GetFramesPerBeat() / DIVAConfig::GetPeriodsPerBeat())); }

	public:
		bool isAlive;
		double timePos;
		double beatsPerMinute;
		vector<int> music;
		int resource;
		vector<Note> notes;
	};

	//----------------------------------------------------------------------------------------------------
public:
	DIVAMap();
	DIVAMap(const char* _filePath, const char* _basePath);
	virtual ~DIVAMap();

	bool LoadMap(const char* _filePath, const char* _basePath);

private:
	// map info
	string m_toolVersion;
	string m_mapName;
	string m_noterName;
	string m_authorName;

	int m_level;
	int m_difficulty;
	float m_beatsPerMinute;
	int m_frameNum;

	vector<Frame> m_frames;
	map<int, string> m_music;
	map<int, string> m_resources;

	int m_chanceTimeStart;
	int m_chanceTimeEnd;

	// runtime data
	int m_framePos;
	int m_frameForwardPos;

};