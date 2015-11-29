#pragma once

#include <string>
#include <vector>
#include <map>
#include "DIVAMap.h"
using namespace std;


//----------------------------------------------------------------------------------------------------
struct DIVAMusicInfo
{
public:
	string musicName;
	string musicPath;
	map<DIVADifficulty, string> musicMapsPath;
	map<DIVADifficulty, DIVAMap> musicMaps;
};

//----------------------------------------------------------------------------------------------------
class DIVAMusicList
{
public:
	DIVAMusicList();

	vector<DIVAMusicInfo>* GetMusicList();
	const DIVAMap* GetMap(unsigned int _listId, DIVADifficulty _difficulty);

private:
	bool LoadMusicList();

private:
	vector<DIVAMusicInfo> m_musicList;
};