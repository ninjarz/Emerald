#include "DIVAHelper.h"
#if _DIVA_

#include <string>
#include <map>
#include <vector>
using namespace std;

typedef int RESOURCE;
typedef int WAV;
const int NOSOURCE = -1;
const int GRAPH = 0;
const int VIDEO = 1;
const int NOWAV = -1;
const int BGSNum = 10;
#define NOTE_NUM 8
#define NOTE_PER_PERIOD 192
#define TIME_PER_PERIOD 4
enum UNIT{ UNIT_CIRCLE = 0, UNIT_RECT = 1, UNIT_CROSS = 2, UNIT_TRIANGLE = 3, UNIT_RIGHT = 4, UNIT_LEFT = 5, UNIT_DOWN = 6, UNIT_UP = 7 };
enum NOTETYPE{ NOTETYPE_NORMAL, NOTETYPE_STRIP };

class Note
{
public:
	Note() :notePos(0), type(0), x(0), y(0), tailx(0), taily(0), key(0) {}
	void Clear() { type = x = y = tailx = taily = key = 0; }

public:
	int notePos;
	int type, x, y, tailx, taily, key;
	float duration;
};

class Frame
{
public:
	Frame() : isSetBPM(false), timePosition(0), noteNum(0), BPM(0), resource(NOSOURCE), notePos(0) { memset(BGM, NOWAV, sizeof(BGM)); }
	void Clear() { isSetBPM = false, noteNum = 0, BPM = 0, resource = NOSOURCE, notePos = 0, timePosition = 0, memset(BGM, NOWAV, sizeof(BGM)); }

public:
	bool isSetBPM;
	int noteNum;
	double BPM;
	int notePos;
	double timePosition;
	int resource;
	int BGM[BGSNum];
	Note notes[NOTE_NUM];
};

class NoteMap
{
public:
	NoteMap(wchar_t* _fileName);

private:
	double m_second;

	string m_version;
	string m_mapName;
	string m_noterName;
	string m_authorName;

	int m_level;
	int m_hard;
	float m_BPM;
	int m_frameNum;

	vector<Frame> m_frames;
	map<int, string> m_waves;
	map<int, string> m_resources;

	int m_chanceTimeStart;
	int m_chanceTimeEnd;
};


#endif