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


private:
	// UI
	EEScene m_scene;
	EEQuad2D m_backgroundQuad;

	// state
	DIVAState m_state;
};