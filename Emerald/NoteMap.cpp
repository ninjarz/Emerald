#include "NoteMap.h"

#include <fstream>
using namespace std;


NoteMap::NoteMap(wchar_t* _fileName)
	:
	m_second(1.000)
{
	ifstream fin(_fileName);
	if (!fin.is_open())
		return;

	string tmp;
	getline(fin, m_version);
	getline(fin, m_mapName);
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
		// m_frames[pos].IsSetBPM = true;
	}

	// set timePosition
	double lastTime = 0, singleTime = 1;
	int lastBPMIndex = 0;
	for (int i = 0; i < m_frameNum; i++)
	{
		if (m_frames[i].BPM > 0)
		{
			lastTime += (i - lastBPMIndex) * singleTime;
			lastBPMIndex = i;
			singleTime = 60.0 * m_second / (m_frames[i].BPM * (NOTE_PER_PERIOD / TIME_PER_PERIOD));
		}
		m_frames[i].timePosition = lastTime + (i - lastBPMIndex) * singleTime;
		// init notes
		for (int j = 0; j < NOTE_NUM; j++)
			m_frames[i].notes[j].notePos = i;
	}

	// resource num
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
		fin >> m_frames[pos].BGM[bgsNum];
	}

	// note
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		int noteNum = m_frames[pos].noteNum;
		Note &note = m_frames[pos].notes[noteNum];
		fin >> note.type;
		fin >> note.x;
		fin >> note.y;
		fin >> note.tailx;
		fin >> note.taily;
		fin >> note.key;
		if (note.type >= NOTE_NUM)
			fin >> note.duration;
		m_frames[pos].noteNum++;
	}

	// waves
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		getline(fin, m_waves[pos]);
	}

	// resource
	while (true)
	{
		int pos;
		fin >> pos;
		if (pos == -1)
			break;
		getline(fin, m_resources[pos]);
	}

	if (m_version >= "1.0.1.0")
		fin >> m_chanceTimeStart >> m_chanceTimeEnd;
	
	fin.close();
}