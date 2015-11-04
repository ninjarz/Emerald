#pragma once

#include "../../Emerald.h"
#include "DIVAConfig.h"


//----------------------------------------------------------------------------------------------------
class DIVAMenu
{
public:
	DIVAMenu();
	virtual ~DIVAMenu();

	DIVAState Process();

private:
	bool Initialize();

private:
	// UI
	EEScene m_scene;
	EEQuad2D m_backgroundQuad;
	EEQuad2D m_logoQuad;
	EEQuad2D m_circle1Quad;
	EEQuad2D m_circle2Quad;
	EEQuad2D m_road1Quad;
	EEQuad2D m_road2Quad;
	EEQuad2D m_topQuad;
	EEQuad2D m_bottomQuad;
	EEQuad2D m_dpQuad;
	EEIntBoard m_dpBoard;
	EEParticleEmitter m_noteParticle;
	EEButton m_freeModeButton;

	// state
	DIVAState m_state;
};