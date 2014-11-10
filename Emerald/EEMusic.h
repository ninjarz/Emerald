#pragma once
#ifndef _EE_MUSIC_H_
#define _EE_MUSIC_H_

/*
*  0     1 big_endian 0-1
*  1     1 is_signed  0-1
*  2     1 unused     0
*  3-5   3 bits >> 3  0-7 (* 8 = 0-56)
*  6-23 18 rate       0-262143
* 24-31  8 channels   0-255
*/
#pragma warning(disable: 4996)

extern "C"
{
#pragma comment (lib, "avcodec.lib")  
#pragma comment (lib, "avdevice.lib")
#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
#pragma comment (lib, "avutil.lib")
#pragma comment (lib, "postproc.lib")
#pragma comment (lib, "swresample.lib")
#pragma comment (lib, "swscale.lib")
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libswresample/swresample.h>
#include <libavutil/opt.h>
#include <libavutil/audioconvert.h>
#include <libavutil/mathematics.h>
}
#pragma comment(lib,"Winmm.lib")
#include <C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\xaudio2.h>
#include <string>


namespace Emerald
{
	//EEMusic
	//----------------------------------------------------------------------------------------------------
	class EEMusic
	{
	public:
		EEMusic();
		EEMusic(const char* _fileName);
		EEMusic(const EEMusic& _music);
		~EEMusic();

		bool Open(const char* _fileName);
		bool Start(float _begin = 0.0f);
		bool Start(float _begin, float _end, int _times = 1);
		bool Pause();
		bool Stop();
		bool SetVolume(float _volume);
		bool SetSampleRate(int _rate);
		bool SetFrequencyRatio(float _para);

		float GetVolume();
		int GetSampleRate();
		int GetTotalSamples();
		int GetSampled();
		float GetProgress();

	private:
		static bool InitializeMusic();

		bool LoadMusic(const char* _fileName);

	private:
		static bool s_isMusicInitialized;
		static IXAudio2 *s_XAudio2;
		static IXAudio2MasteringVoice *s_masteringVoice;

		IXAudio2SourceVoice *m_sourceVoice;
		XAUDIO2_BUFFER m_buffer;
		int m_channels;
		int m_bitsPerSample;
		int m_samplesPerSec;
		int m_blockAlign;
		int m_avgBytesPerSec;
		int m_totalBytes;
		int m_totalSamples;
		int m_totalTimes;
		int m_beginSamples;
		char *m_data;
	};
}

#endif