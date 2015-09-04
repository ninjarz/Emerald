#pragma once

#include "../../Emerald.h"
using namespace std;

#define BGS_NUM 10
#define NOTE_NUM 8
#define NOTE_PER_PERIOD 192
#define TIME_PER_PERIOD 4
enum NOTE_TYPE { NOTETYPE_NORMAL = 0, NOTETYPE_STRIP = 1 };
enum NOTE_KEY { NOTE_CIRCLE = 0, NOTE_RECT = 1, NOTE_CROSS = 2, NOTE_TRIANGLE = 3, NOTE_RIGHT = 4, NOTE_LEFT = 5, NOTE_DOWN = 6, NOTE_UP = 7 };

class Note
{
public:
	Note() :notePos(0), sound(0), x(0), y(0), tailx(0), taily(0), key(0) {}

public:
	int notePos;
	int sound;
	int key;
	int  x, y;
	float tailx, taily;
	float duration;
};

class Frame
{
public:
	Frame() : isAlive(true), isSetBPM(false), timePos(0), noteNum(0), BPM(0), resource(-1) { memset(music, -1, sizeof(music)); }

	inline double GetSingleTime() { return 60.0 / (BPM * (NOTE_PER_PERIOD / TIME_PER_PERIOD)); }

public:
	bool isAlive;
	double timePos;
	bool isSetBPM;
	double BPM;
	int music[BGS_NUM];
	int resource;
	int noteNum;
	Note notes[NOTE_NUM];
};

class NoteMap
{
public:
	NoteMap();
	NoteMap(const char* _filePath, const char* _basePath);

	Frame* GetFrame();
	int GetFramePos();
	Frame* GetFrameForward();
	int GetFrameForwardPos();
	map<int, string>& GetMusicPaths();
	string GetMusicPath(int _i);

private:
	double m_second;

	// map data
	string m_version;
	string m_mapName;
	string m_noterName;
	string m_authorName;

	int m_level;
	int m_hard;
	float m_BPM;
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
