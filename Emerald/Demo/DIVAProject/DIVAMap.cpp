#include "DIVAMap.h"
#include <string>
#include <fstream>
#include "../../Emerald.h"
using namespace std;


DIVAMap::DIVAMap()
{
}

DIVAMap::DIVAMap(const char* _filePath, const char* _basePath)
{
	LoadMap(_filePath, _basePath);
}

DIVAMap::~DIVAMap()
{

}

bool DIVAMap::LoadMap(const char* _filePath, const char* _basePath)
{
	ifstream fin(_filePath);
	if (!fin.is_open())
		return false;

	string tmp;
	string basePath = _basePath;
	getline(fin, m_toolVersion);
	getline(fin, m_mapName);
	getline(fin, m_noterName); // ..
	getline(fin, m_authorName); // ..
	getline(fin, tmp); // ..
	getline(fin, tmp); // ..

	fin >> m_level; // ..
	fin >> m_difficulty;
	fin >> m_beatsPerMinute;
	fin >> m_frameNum; // beats num
	m_frameNum *= DIVAConfig::GetFramesPerBeat();

	// init frames
	m_frames.resize(m_frameNum);

	// set beats per minute
	while (true)
	{
		int framePos;
		fin >> framePos;
		if (framePos == -1)
			break;

		fin >> m_frames[framePos].beatsPerMinute;
		if (m_toolVersion <= "1.0.4.7")
		{
			double decimal = m_frames[framePos].beatsPerMinute - floor(m_frames[framePos].beatsPerMinute);
			if (decimal > 0.8)
				m_frames[framePos].beatsPerMinute = ceil(m_frames[framePos].beatsPerMinute);
			else if (decimal < 0.2)
				m_frames[framePos].beatsPerMinute = floor(m_frames[framePos].beatsPerMinute);
		}
	}

	// set time pos
	double timePos = 0, singleTime = 1;
	for (int i = 0; i < m_frameNum; ++i)
	{
		if (m_frames[i].beatsPerMinute > 0)
		{
			singleTime = 60.0 / (m_frames[i].beatsPerMinute * (DIVAConfig::GetFramesPerBeat() / DIVAConfig::GetPeriodsPerBeat()));
		}

		m_frames[i].timePos = timePos;
		timePos += singleTime;
	}

	// resource
	while (true)
	{
		int framePos;
		fin >> framePos;
		if (framePos == -1)
			break;

		fin >> m_frames[framePos].resource;
	}

	// music
	while (true)
	{
		int framePos;
		fin >> framePos;
		if (framePos == -1)
			break;

		int musicId;
		fin >> musicId;
		fin >> m_frames[framePos].music[musicId];
	}

	// note info
	while (true)
	{
		int framePos;
		fin >> framePos;
		if (framePos == -1)
			break;

		Note note;
		int key(0);
		fin >> key;
		note.keyType = (NoteType)(key / DIVAConfig::GetNoteNum());
		note.key = (NoteKey)(key % DIVAConfig::GetNoteNum());
		fin >> note.x;
		note.x = (note.x + 3) * DIVAConfig::GetNoteWidth();
		fin >> note.y;
		note.y = (note.y + 5) * DIVAConfig::GetNoteHeight();
		fin >> note.tailX;
		note.tailX = (note.tailX / 12) * DIVAConfig::GetNoteWidth(); // ..
		fin >> note.tailY;
		note.tailY = (note.tailY / 12) * DIVAConfig::GetNoteHeight(); // ..
		fin >> note.sound;
		if (note.keyType == NoteType::NOTE_STRIP)
			fin >> note.duration;

		m_frames[framePos].notes.push_back(note);
	}

	// music file
	while (true)
	{
		int musicId;
		fin >> musicId;
		if (musicId == -1)
			break;

		string music;
		getline(fin, music);
		music = basePath + trim(music);
		m_music[musicId] = music;
	}

	// resource file
	while (true)
	{
		int resourceId;
		fin >> resourceId;
		if (resourceId == -1)
			break;

		string resource;
		getline(fin, resource);
		resource = basePath + trim(resource);
		m_resources[resourceId] = resource;
	}

	if (m_toolVersion >= "1.0.1.0")
		fin >> m_chanceTimeStart >> m_chanceTimeEnd;

	fin.close();
	return true;
}