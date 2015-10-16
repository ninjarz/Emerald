#pragma once
#ifndef _EE_VIDEO_H_
#define _EE_VIDEO_H_

#include <vector>
#include "EETexture.h"


namespace Emerald
{
	// EEVideoData
	//----------------------------------------------------------------------------------------------------
	struct EEVideoData
	{


	};

	// EEVideo
	//----------------------------------------------------------------------------------------------------
	class EEVideo
	{
	public:
		static bool InitializeVideo();

	protected:
		static bool s_isVideoInitialized;

	public:
		EEVideo();

		bool Open(const char* _fileName);
		bool Play();
		bool LoadVideo(const char* _fileName);

		std::vector<EEBitmap>& GetData();

	protected:
		// info
		int m_width;
		int m_height;
		double m_frameRate;
		double m_totalTime;

		// data
		std::vector<EEBitmap> m_data; // X, Too Large!
	};
}

#endif