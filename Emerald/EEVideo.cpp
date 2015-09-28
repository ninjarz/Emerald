#include "EEVideo.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	EEVideo::EEVideo()
	{

	}

	bool EEVideo::Open(const char* _fileName)
	{
		return LoadVideo(_fileName);
	}

	bool EEVideo::Play()
	{

		return true;
	}

	bool EEVideo::LoadVideo(const char* _fileName)
	{

		return true;
	}
}