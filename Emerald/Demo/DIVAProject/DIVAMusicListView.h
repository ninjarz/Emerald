#pragma once

#include "../../Emerald.h"
#include "DIVAMusicList.h"


//----------------------------------------------------------------------------------------------------
class DIVAMusicListView : public EEListView
{
public:
	DIVAMusicListView(const Rect_Float& _area);

private:
	bool Initialize();
	void Shutdown();

private:
	DIVAMusicList m_musicList;
};