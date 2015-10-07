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
#if 0
#pragma comment(lib,"xaudio2.lib")
#include <xaudio2.h>
#elif 1
#include <C:/Program Files (x86)/Microsoft DirectX SDK (June 2010)/Include/XAudio2.h>
#endif
#include <vector>
#include <list>
#include <queue>
#include <string>
#include <mutex>
#include "EEThread.h"


namespace Emerald
{
	class EEMusic;
	class EEMusicCallBack;

	// EEMusicState
	//----------------------------------------------------------------------------------------------------
	enum EEMusicState
	{
		EE_MUSIC_DEFAULT = 0,
		EE_MUSIC_PLAYING,
		EE_MUSIC_NO_BUFFER,
		EE_MUSIC_PAUSE,
		EE_MUSIC_END
	};

	// EEMusicCellState
	//----------------------------------------------------------------------------------------------------
	enum EEMusicCellState
	{
		EE_MUSIC_CELL_DEFAULT = 0,
		EE_MUSIC_CELL_END
	};

	// EEMusicCell
	//----------------------------------------------------------------------------------------------------
	struct EEMusicCell
	{
		inline EEMusicCell() : data(nullptr), begin(0), playLength(0), flag(EE_MUSIC_CELL_DEFAULT) {}
		inline EEMusicCell(std::pair<int, std::string>* _data) : data(_data), begin(0), playLength(0), flag(EE_MUSIC_CELL_DEFAULT) {}
		inline EEMusicCell(std::pair<int, std::string>* _data, int _begin) : data(_data), begin(_begin), playLength(0), flag(EE_MUSIC_CELL_DEFAULT) {}
		inline EEMusicCell(std::pair<int, std::string>* _data, int _begin, int _len) : data(_data), begin(_begin), playLength(_len), flag(EE_MUSIC_CELL_DEFAULT) {}
		inline EEMusicCell(std::pair<int, std::string>* _data, int _begin, int _len, EEMusicCellState _state) : data(_data), begin(_begin), playLength(_len), flag(_state) {}

		std::pair<int, std::string> *data; // mirror of m_data
		int begin;
		int playLength;
		EEMusicCellState flag;
	};


	// EEMusicCallBack
	//----------------------------------------------------------------------------------------------------
	class EEMusicCallBack :public IXAudio2VoiceCallback
	{
	public:
		inline EEMusicCallBack(EEMusic* _music) : IXAudio2VoiceCallback(), music(_music) {}
		inline ~EEMusicCallBack() {}

		// important
		virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnBufferEnd(void* _context);
		// keep
		virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnBufferStart(void* _context);
		virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceProcessingPassEnd();
		virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 _samplesRequired);
		virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnStreamEnd();
		virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnLoopEnd(void* _context);
		virtual COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE OnVoiceError(void* _context, HRESULT _error);

	public:
		EEMusic* music;
	};

	// EEMusic
	// an instance of EEMusic can just holds one kind of formats
	// XAUDIO2_MAX_QUEUED_BUFFERS
	//----------------------------------------------------------------------------------------------------
	class EEMusic
	{
		friend EEMusicCallBack;
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
		bool SyncLoadMusic(const char* _fileName);
		bool SyncLoadMusic(const std::string& _fileName);
		bool AsyncLoadMusic(const char* _fileName);
		bool AsyncLoadMusic(const std::string& _fileName);

		bool SetVolume(float _volume);
		bool SetSampleRate(int _rate);
		bool SetFrequencyRatio(float _para);

		const WAVEFORMATEX& GetFormat();
		int GetBitsPerSample();
		int GetChannels();
		float GetVolume();
		int GetSampleRate();
		int GetTotalSamples();
		int GetSampled();
		double GetTotalTime();
		float GetProgress();
		double GetProgressTime();
		std::string GetCurrentSample(int _count = 1);

	protected:
		bool PushBuffer(const EEMusicCell& _buffer);
		bool SubmitBuffer();

	protected:
		// state
		IXAudio2SourceVoice *m_sourceVoice;
		EEMusicCallBack m_musicCallBack;
		WAVEFORMATEX m_format;
		int m_totalBytes;
		int m_totalSamples;
		double m_totalTime;
		int m_beginSamples; // offsetSamplers
		EEMusicState m_state;

		// data
		std::list<std::pair<int, std::string>> m_data; // id, data
		std::queue<EEMusicCell> m_playList; // need mutex
		std::mutex m_playListMutex;
		boost::thread *m_loader;
	};
}

#endif