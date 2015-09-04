#include "DIVAHelper.h"
#if _DIVA_
#include "DIVAMusicList.h"

#include <Shlobj.h>
#include <iostream>
#include <string>

DIVAMusicList::DIVAMusicList()
{

}

bool DIVAMusicList::Initialize()
{
	return LoadMusicList();
}

vector<DIVAMusicInfo>* DIVAMusicList::GetMusicList()
{
	return &m_musicList;
}

NoteMap* DIVAMusicList::GetNoteMap(unsigned int _listId, unsigned int _degree)
{
	if (_listId < 0 || m_musicList.size() <= _listId)
		return nullptr;

	auto& info = m_musicList[_listId].musicMapsInfo;
	auto it = info.find(_degree);
	if (it == info.end())
	{
		info[_degree] = NoteMap(m_musicList[_listId].musicMaps[_degree].c_str(), m_musicList[_listId].musicPath.c_str());
	}

	return &info[_degree];
}

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
							info.musicMaps[0] = info.musicPath + mapData.cFileName;
						else if (degree == "Normal")
							info.musicMaps[1] = info.musicPath + mapData.cFileName;
						else if (degree == "Hard")
							info.musicMaps[2] = info.musicPath + mapData.cFileName;
						else
							info.musicMaps[3] = info.musicPath + mapData.cFileName;
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

#endif