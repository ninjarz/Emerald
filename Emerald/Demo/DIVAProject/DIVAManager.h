#pragma once

#define _EE_MEMORYLEAK_ON_
#include "../../Emerald.h"

#include <string>
#include <vector>
#include <map>
#include "DIVAConfig.h"
using namespace std;


//----------------------------------------------------------------------------------------------------
class DIVAManager
{
public:
	DIVAManager();
	virtual ~DIVAManager();

	bool Process();

private:
	bool Initialize();

private:
	// state
	DIVAState m_state;
	int m_currentMusic;
	int m_currentDegree;

	// data
};