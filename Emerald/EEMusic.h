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
#pragma comment(lib,"xaudio2.lib")
#include <xaudio2.h>
#include <vector>
#include <list>
#include <string>
#include <boost/thread/thread.hpp>

namespace Emerald
{
	// EEMusicCallBack
	//----------------------------------------------------------------------------------------------------
	class EEMusicCallBack :public IXAudio2VoiceCallback
	{
	public:
		inline EEMusicCallBack(EEMusic* _music) : IXAudio2VoiceCallback() { music = _music; }
		inline ~EEMusicCallBack() {}

		// important
		 virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnBufferEnd(void* _context) {}
		// keep
		 virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnBufferStart(void* _context) {}
		 virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() {}
		 virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 _samplesRequired) {}
		 virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnStreamEnd() {}
		 virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnLoopEnd(void* _context) {}
		 virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceError(void* _context, HRESULT _error) {}

	public:
		EEMusic* music;
	};

	// EEMusic
	// an instance of EEMusic can just holds one kind of formats
	// XAUDIO2_MAX_QUEUED_BUFFERS
	//----------------------------------------------------------------------------------------------------
	class EEMusic
	{
	public:
		static bool InitializeMusic();

	protected:
		static bool s_isMusicInitialized;
		static IXAudio2 *s_XAudio2;
		static IXAudio2MasteringVoice *s_masteringVoice;

	public:
		EEMusic();
		EEMusic(const WAVEFORMATEX& _format);
		EEMusic(const char* _fileName);
		EEMusic(const std::string& _fileName);
		EEMusic(const EEMusic& _music);
		virtual ~EEMusic();

		bool Open(const char* _fileName);
		bool Open(const std::string& _fileName);
		bool Start();
		bool Play(float _begin = 0.f);
		bool Play(float _begin, float _end, int _times = 1);
		bool Pause();
		bool Stop();
		bool AddBuffer(const char* _buffer, unsigned int _size);
		bool AddBuffer(const std::string& _buffer);
		bool LoadMusic(const char* _fileName);
		bool LoadMusic(const std::string& _fileName);
		bool AsyncLoadMusic(const char* _fileName);

		bool SetVolume(float _volume);
		bool SetSampleRate(int _rate);
		bool SetFrequencyRatio(float _para);

		const WAVEFORMATEX& GetFormat();
		float GetVolume();
		int GetSampleRate();
		int GetTotalSamples();
		int GetSampled();
		double GetTotalTime();
		float GetProgress();
		double GetProgressTime();
		char* GetSampleData(int _num);

	protected:
		IXAudio2SourceVoice *m_sourceVoice;
		EEMusicCallBack m_musicCallBack;
		WAVEFORMATEX m_format;
		int m_totalBytes;
		int m_totalSamples;
		double m_totalTime;
		int m_beginSamples;
		std::list<std::string> m_data;
		boost::thread *m_loader;
	};
}

#endif