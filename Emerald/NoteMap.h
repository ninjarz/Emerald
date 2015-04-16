#include "DIVAHelper.h"
#if _DIVA_

#include <string>
#include <map>
using namespace std;

typedef int RESOURCE;
typedef int WAV;
const int NOSOURCE = -1;
const int GRAPH = 0;
const int VIDEO = 1;
const int NOWAV = -1;
const int BGSNum = 10;
const int NOTENum = 8;
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
	Note note[NOTENum];
};

class TimeTable
{
public:
	TimeTable() :BPM(0), frameNum(0), framePos(0), notesPos(0) { frames = NULL; }
	void Clear() { BPM = 0, frameNum = framePos = notesPos = 0; }
	//void Prework();
	//bool NextFrame();
	//bool NextNotes();
	Frame* GetNotes() { if (notesPos == frameNum)return NULL; return &frames[notesPos]; }
	Frame* GetFrame() { if (framePos == frameNum)return NULL; return &frames[framePos]; }

private:
	double BPM;
	int frameNum;
	int framePos;
	int notesPos;
	//Frame _frame[PeriodNum*notePerPeriod];
	Frame *frames;
	friend class NoteMap;
};

class NoteMap
{
public:
	NoteMap() {}
	NoteMap(string filename, string baseDirec) { SetNew(filename, baseDirec); }

	static NoteMap& Instance(void) { static NoteMap instance; return instance; }

	void Init() {}
	void SetNew(string filename, string baseDirec);
	void Resource_read();

	//bool NextFrame() { return _timeTable.NextFrame(); }
	//bool NextNotes() { return _timeTable.NextNotes(); }
	Frame* GetFrame() { return _timeTable.GetFrame(); }
	Frame* GetNotes() { return _timeTable.GetNotes(); }

	string GetWav(WAV id) { if (wav.find(id) == wav.end())return ""; return wav[id]; }
	string GetResource(RESOURCE id) { if (resource.find(id) == resource.end())return ""; return resource[id]; }

	void Clear() { _chanceTimeStart = _chanceTimeEnd = -1, _timeTable.Clear(); }
	int GetFramePos() { return _timeTable.framePos; }
	int GetNotesPos() { return _timeTable.notesPos; }
	int GetTotalPos() { return _timeTable.frameNum; }
	int GetLevel() { return _level; }
	string GetMapName() { return _mapName; }
	double GetBPM() { return _timeTable.BPM; }
	double Cal_Single(const double &_bpm) { return 0;/*60.0*base::FPS/(_bpm/timePerPeriod*notePerPeriod);*/ }

public:
	int _chanceTimeStart, _chanceTimeEnd;

private:
	string EditorVer;
	string _mapName, _noterName, _authorName, _musicStyle;
	int _level, _hard;
	TimeTable _timeTable;
	map<int, string> wav;
	map<int, string> resource;
};


#endif