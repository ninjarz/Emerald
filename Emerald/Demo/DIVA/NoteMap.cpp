#include "DIVAHelper.h"
#if _DIVA_
#include "NoteMap.h"

#include <fstream>
using namespace std;


//----------------------------------------------------------------------------------------------------
NoteMap::NoteMap()
	:
	m_second(1.000),
	m_framePos(0),
	m_frameForwardPos(0)
{

}

//----------------------------------------------------------------------------------------------------
NoteMap::NoteMap(wchar_t* _fileName)
	:
	m_second(1.000),
	m_framePos(0),
	m_frameForwardPos(0)
{
	ifstream fin(_fileName);
	if (!fin.is_open())
		return;

	string tmp;
	getline(fin, m_version);
	getline(fin, m_mapName);
	string basePath = string("Song\\") + m_mapName + "\\";
	getline(fin, m_noterName);
	getline(fin, m_authorName);
	getline(fin, tmp);
	getline(fin, tmp);

	fin >> m_level;
	fin >> m_hard;
	fin >> m_BPM;
	fin >> m_frameNum;
	m_frameNum *= NOTE_PER_PERIOD;

	// init frame
	m_frames.resize(m_frameNum);

	// set BPM
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		fin >> m_frames[pos].BPM;
		if (m_version <= "1.0.4.7")
		{
			double digit = m_frames[pos].BPM - floor(m_frames[pos].BPM);
			if (digit > 0.8)
				m_frames[pos].BPM = ceil(m_frames[pos].BPM);
			else if (digit < 0.2)
				m_frames[pos].BPM = floor(m_frames[pos].BPM);
		}
		m_frames[pos].isSetBPM = true;
	}

	// set timePosition
	double lastTime = 0, singleTime = 1;
	for (int i = 0; i < m_frameNum; i++)
	{
		if (m_frames[i].BPM > 0)
		{
			singleTime = 60.0 * m_second / (m_frames[i].BPM * (NOTE_PER_PERIOD / TIME_PER_PERIOD));
		}
		m_frames[i].timePos = lastTime;
		lastTime += singleTime;
		// init notes
		for (int j = 0; j < NOTE_NUM; j++)
			m_frames[i].notes[j].notePos = i;
	}

	// resource
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		fin >> m_frames[pos].resource;
	}

	// BGS
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		int bgsNum;
		fin >> bgsNum;
		fin >> m_frames[pos].music[bgsNum];
	}

	int pixelX = EEGetWidth() / 40;
	int pixelY = EEGetHeight() / 22;
	// note
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		int noteNum = m_frames[pos].noteNum;
		Note &note = m_frames[pos].notes[noteNum];
		fin >> note.key;
		fin >> note.x;
		note.x = (note.x + 3) * pixelX;
		fin >> note.y;
		note.y = (note.y + 5) * pixelY;
		fin >> note.tailx;
		note.tailx = (note.tailx / 12) * pixelX;
		fin >> note.taily;
		note.taily = (note.taily / 12) * pixelY;
		fin >> note.sound;
		if (note.key >= NOTE_NUM)
			fin >> note.duration;
		m_frames[pos].noteNum++;
	}

	// music file
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		string music;
		getline(fin, music);
		music = basePath + trim(music);
		m_music[pos] = music;
	}

	// resource file
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		string src;
		getline(fin, src);
		src = basePath + trim(src);
		m_resources[pos] = src;
	}

	if (m_version >= "1.0.1.0")
		fin >> m_chanceTimeStart >> m_chanceTimeEnd;
	
	fin.close();
}

//----------------------------------------------------------------------------------------------------
Frame* NoteMap::GetFrame()
{
	if ((unsigned int)m_framePos < m_frames.size())
		return &m_frames[m_framePos++];
	else
		return nullptr;
}

//----------------------------------------------------------------------------------------------------
int NoteMap::GetFramePos()
{
	return m_framePos;
}

//----------------------------------------------------------------------------------------------------
Frame* NoteMap::GetFrameForward()
{
	if ((unsigned int)m_frameForwardPos < m_frames.size())
	if (m_frameForwardPos < m_framePos + NOTE_PER_PERIOD)
		return &m_frames[m_frameForwardPos++];
	return nullptr;
}

//----------------------------------------------------------------------------------------------------
int NoteMap::GetFrameForwardPos()
{
	return m_frameForwardPos;
}

//----------------------------------------------------------------------------------------------------
map<int, string>& NoteMap::GetMusicPaths()
{
	return m_music;
}

//----------------------------------------------------------------------------------------------------
string NoteMap::GetMusicPath(int _i)
{
	return m_music[_i];
}

#endif