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
	map<int, string> musicMaps;
	map<int, NoteMap> musicMapsInfo;
};

class DIVAMusicList
{
public:
	DIVAMusicList();

	bool Initialize();
	vector<DIVAMusicInfo>* GetMusicList();
	NoteMap* GetNoteMap(unsigned int _listId, unsigned int _degree);

protected:
	bool LoadMusicList();

protected:
	vector<DIVAMusicInfo> m_musicList;
};