#pragma once

#include <string>
#include <vector>
#include <map>
#include "NoteMap.h"
using namespace std;

struct DIVAMusicInfo
{
public:
	string musicName;
	string musicPath;
	map<int, string> musicMapPaths;
	map<int, NoteMap> musicMapData;
};

class DIVAMusicList
{
public:
	DIVAMusicList();

	bool Initialize();
	vector<DIVAMusicInfo>* GetMusicList();
	bool SelectMusic(unsigned int _listId);
	bool SelectDegree(unsigned int _degree);
	bool SelectNoteMap(unsigned int _listId, unsigned int _degree);
	const NoteMap* GetNoteMap();
	const NoteMap* GetNoteMap(unsigned int _listId, unsigned int _degree);

protected:
	bool LoadMusicList();

protected:
	vector<DIVAMusicInfo> m_musicList;
	unsigned int m_currentMusic;
	unsigned int m_currentDegree;
};