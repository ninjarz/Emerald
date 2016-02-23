#include "DIVAMusicList.h"
#include <Shlobj.h>
#include <string>


//----------------------------------------------------------------------------------------------------
DIVAMusicList::DIVAMusicList()
{
	LoadMusicList();
}

//----------------------------------------------------------------------------------------------------
vector<DIVAMusicInfo>* DIVAMusicList::GetMusicList()
{
	return &m_musicList;
}

//----------------------------------------------------------------------------------------------------
const DIVAMap* DIVAMusicList::GetMap(unsigned int _listId, DIVADifficulty _difficulty)
{
	if (_listId < 0 || m_musicList.size() <= _listId)
		return nullptr;

	auto& paths = m_musicList[_listId].musicMapsPath;
	auto& pathsIt = paths.find(_difficulty);
	if (pathsIt == paths.end())
		return nullptr;

	auto& maps = m_musicList[_listId].musicMaps;
	auto mapIt = maps.find(_difficulty);
	if (mapIt == maps.end())
	{
		maps[_difficulty] = DIVAMap(pathsIt->second.c_str(), m_musicList[_listId].musicPath.c_str());
	}

	return &maps[_difficulty];
}

//----------------------------------------------------------------------------------------------------
bool DIVAMusicList::LoadMusicList()
{
	char pathBase[MAX_PATH] = { 0 };
	GetCurrentDirectoryA(MAX_PATH, pathBase);

	// find music directory
	HANDLE musicHandle;
	WIN32_FIND_DATAA musicData;
	string path(pathBase);
	path += "/Song/";
	musicHandle = FindFirstFileA((path + "*.*").c_str(), &musicData);
	if (musicHandle != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (musicData.cFileName[0] == '.')
				continue;

			if (musicData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				DIVAMusicInfo info;
				info.musicName = musicData.cFileName;
				info.musicPath = path + musicData.cFileName + '/';
				// find diva
				HANDLE mapHandle;
				WIN32_FIND_DATAA mapData;
				mapHandle = FindFirstFileA((info.musicPath + "*.diva").c_str(), &mapData);
				if (mapHandle != INVALID_HANDLE_VALUE)
				{
					do
					{
						string degree = mapData.cFileName;
						auto begin = degree.find_last_of('_') + 1;
						auto end = degree.find_last_of('.') - 1;
						degree = degree.substr(begin, end - begin + 1);
						if (degree == "Easy")
							info.musicMapsPath[DIVA_EASY] = info.musicPath + mapData.cFileName;
						else if (degree == "Normal")
							info.musicMapsPath[DIVA_NORMAL] = info.musicPath + mapData.cFileName;
						else if (degree == "Hard")
							info.musicMapsPath[DIVA_HARD] = info.musicPath + mapData.cFileName;
						else
							info.musicMapsPath[DIVA_FUCK] = info.musicPath + mapData.cFileName;
					} while (FindNextFileA(mapHandle, &mapData));
				}

				// save
				m_musicList.push_back(info);
			}
		} while (FindNextFileA(musicHandle, &musicData));

		return true;
	}
	return false;
}
