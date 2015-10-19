#pragma once
#ifndef _EE_VIDEO_H_
#define _EE_VIDEO_H_

#pragma warning(disable: 4996)
extern "C"
{
#pragma comment (lib, "avcodec.lib")  
#pragma comment (lib, "avdevice.lib")
#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
#pragma comment (lib, "avutil.lib")
#pragma comment (lib, "swresample.lib")
#pragma comment (lib, "swscale.lib")
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/avutil.h>
#include <libavutil/mathematics.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}
#include <vector>
#include "EEQuad2D.h"
#include "EEMusic.h"


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
		EEVideo(const Rect_Float& _rect);
		~EEVideo();

		virtual bool Update();
		virtual bool Render();
		virtual bool Process();
		bool Open(const char* _fileName);
		bool Play();

		int GetPacketSize();
		EETexture GetFrame(int _num);

	protected:
		bool LoadVideo(const char* _fileName);

	protected:
		// info
		AVFormatContext *m_formatContext;
		int m_streamIndex;
		AVCodecContext *m_codecContext;
		SwsContext *m_swsContext;
		AVFrame	*m_frameRGBA;
		int m_width;
		int m_height;
		double m_frameRate;
		double m_totalTime;

		bool m_isRunning;
		double m_startTime;
		int m_currentFrame;

		// data
		EEQuad2D m_screen;
		std::vector<AVPacket*> m_data;
		EEMusic m_music;
	};
}

#endif