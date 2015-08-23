#pragma once

#include <list>
#include "NoteMap.h"
using namespace std;

// DIVANoteState
//----------------------------------------------------------------------------------------------------
enum DIVANoteState
{
	DIVA_NOTE_DEFAULT = 0,

	DIVA_NOTE_COOL,
	DIVA_NOTE_FINE,
	DIVA_NOTE_SAD,
	DIVA_NOTE_SAFE,
	DIVA_NOTE_WORST,

	DIVA_NOTE_STRIP_COOL,
	DIVA_NOTE_STRIP_FINE,
	DIVA_NOTE_STRIP_SAD,
	DIVA_NOTE_STRIP_SAFE,
	DIVA_NOTE_STRIP_WORST

};


// DIVANote
//----------------------------------------------------------------------------------------------------
class DIVANote : public EEQuad2D
{
public:
	DIVANote(const Note& _note, double _totalTime, double singleTime, EETexture& _tex, EETexture& _stripTex);

	bool Update(double _deltaTime);

public:
	// data
	int m_type;
	unsigned int m_key;
	int m_x, m_y, m_tailx, m_taily;
	EEQuad2D m_note;
	EECurve2D m_strip;
	EEQuad2D m_stripNote;

	// state
	DIVANoteState m_state;
	double m_actionTime;
	double m_totalTime;
	double m_restTime;

	double m_totalDuration;
	double m_restDuration;
};


class DIVAMana
{
public:
	DIVAMana(wchar_t* _fileName);
	~DIVAMana();

	bool Start();
	bool Process();

private:
	bool m_isStart;

	double m_startTime;  // globle time
	double m_currentTime;
	double m_totalTime;
	double m_noteTime;
	double m_noteTimeForward;
	double m_singleTime;
	double m_distanceTime;

	NoteMap m_noteMap;
	map<int, EEMusic*> m_music;
	std::list<DIVANote> m_notes;
	EEAnimationEmitter m_emitter;

	// resource
	EETexture circleTex;
	EETexture crossTex;
	EETexture squareTex;
	EETexture triangleTex;
	EETexture rightTex;
	EETexture leftTex;
	EETexture downTex;
	EETexture upTex;
	EETexture stripBlueTex;
	EETexture stripGreenTex;
	EETexture stripPinkTex;
	EETexture stripRedTex;
};