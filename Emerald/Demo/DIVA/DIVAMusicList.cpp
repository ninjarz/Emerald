#include "DIVAHelper.h"
#if _DIVA_
#include "DIVAMusicList.h"

#include <Shlobj.h>
#include <iostream>
#include <string>

DIVAMusicList::DIVAMusicList()
	:
	m_currentMusic(0),
	m_currentDegree(0)
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

bool DIVAMusicList::SelectMusic(unsigned int _listId)
{
	m_currentMusic = _listId;

	return true;
}

bool DIVAMusicList::SelectDegree(unsigned int _degree)
{
	m_currentDegree = _degree;

	return true;
}

bool DIVAMusicList::SelectNoteMap(unsigned int _listId, unsigned int _degree)
{
	m_currentMusic = _listId;
	m_currentDegree = _degree;

	return true;
}

const NoteMap* DIVAMusicList::GetNoteMap()
{
	return GetNoteMap(m_currentMusic, m_currentDegree);
}

const NoteMap* DIVAMusicList::GetNoteMap(unsigned int _listId, unsigned int _degree)
{
	if (_listId < 0 || m_musicList.size() <= _listId)
		return nullptr;
	auto& paths = m_musicList[_listId].musicMapPaths;
	auto& pathsIt = paths.find(_degree);
	if (pathsIt == paths.end())
		return nullptr;

	auto& data = m_musicList[_listId].musicMapData;
	auto dataIt = data.find(_degree);
	if (dataIt == data.end())
	{
		data[_degree] = NoteMap(m_musicList[_listId].musicMapPaths[_degree].c_str(), m_musicList[_listId].musicPath.c_str());
	}

	return &data[_degree];
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
							info.musicMapPaths[0] = info.musicPath + mapData.cFileName;
						else if (degree == "Normal")
							info.musicMapPaths[1] = info.musicPath + mapData.cFileName;
						else if (degree == "Hard")
							info.musicMapPaths[2] = info.musicPath + mapData.cFileName;
						else
							info.musicMapPaths[3] = info.musicPath + mapData.cFileName;
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