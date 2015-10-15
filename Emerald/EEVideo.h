#pragma once
#ifndef _EE_VIDEO_H_
#define _EE_VIDEO_H_

#include <vector>
#include "EETexture.h"


namespace Emerald
{
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

		std::vector<EETexture>& GetData();

	protected:
		// data
		std::vector<EETexture> m_data;
	};
}

#endif