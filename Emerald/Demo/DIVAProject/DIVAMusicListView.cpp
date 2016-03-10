#include "DIVAMusicListView.h"


//----------------------------------------------------------------------------------------------------
DIVAMusicListView::DIVAMusicListView(const Rect_Float& _area)
	:
	EEListView(_area),
	m_musicList()
{
	Initialize();
}

//----------------------------------------------------------------------------------------------------
bool DIVAMusicListView::Initialize()
{
	for (DIVAMusicInfo musicInfo : *m_musicList.GetMusicList())
	{
		Sleep(0); // Todo
	}

	return true;
}

//----------------------------------------------------------------------------------------------------
void DIVAMusicListView::Shutdown()
{

}