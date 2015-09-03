#pragma once

#include <string>
#include <vector>
#include <map>
using namespace std;

struct DIVAMusicInfo
{
public:
	string musicName;
	map<int, string> musicMaps;
};

class DIVAMusicList
{
public:
	DIVAMusicList();

	bool Initialize();

protected:
	bool GetMusicList();

protected:
	vector<DIVAMusicInfo> m_musicList;
};