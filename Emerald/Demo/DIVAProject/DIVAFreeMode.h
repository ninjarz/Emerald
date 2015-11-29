#pragma once

#include "../../Emerald.h"
#include "DIVAConfig.h"


//----------------------------------------------------------------------------------------------------
class DIVAFreeMode
{
public:
	DIVAFreeMode();
	virtual ~DIVAFreeMode();

	DIVAState Process();

private:
	bool Initialize();
	void Shutdown();

	bool SetDifficulty(int _difficulty);

private:
	// state
	DIVAState m_state;
	int m_difficulty;

	// UI
	EEScene m_scene;
	EESlide m_backgroundSlide;
	EESlide m_circleSlide;
	EESlide m_line1Slide;
	EESlide m_line2Slide;
	EESlide m_starSlide;
	EEQuad2D m_itemZoneQuad;
	EEButton m_itemButton;
	EEQuad2D m_topQuad;
	EEQuad2D m_bottomQuad;
	EEQuad2D m_difficultyFrame1Quad;
	EEQuad2D m_difficultyFrame2Quad;
	EEButton m_easyButton;
	EEButton m_normalButton;
	EEButton m_hardButton;
	EEButton m_extremeButton;
};