#pragma once
#ifndef _EE_RECORDER_H_
#define _EE_RECORDER_H_

#pragma comment(lib,"Winmm")
#include <windows.h>
#include <mmsystem.h>
#include <vector>
#include <string>
#include "EEMusic.h"

#define EE_RECORDER_SAMPLE_POOR 8000
#define EE_RECORDER_SAMPLE_LOW 11025
#define EE_RECORDER_SAMPLE_NORMAL 22050
#define EE_RECORDER_SAMPLE_HIGH 44100
#define EE_RECORDER_BIT 16
#define EE_RECORDER_CHANNELS 2

#define EE_RECORDER_FRAME_NUM 10
#define EE_RECORDER_FRAME_SIZE 960

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EERecorder
	//----------------------------------------------------------------------------------------------------
	class EERecorder : public EEMusic
	{
	public:
		static std::vector<EERecorder*> s_recorders;

		static void CALLBACK RecorderInProc(HWAVEIN _waveIn, UINT _msg, DWORD_PTR _instance, DWORD_PTR _param1, DWORD_PTR _param2);
		static EERecorder* FindRecorder(HWAVEIN _waveIn);

	public:
		EERecorder();
		~EERecorder();

		void Start();
		bool Play();

	protected:
		bool AddBuffer(const char* _buffer);

	protected:
		HWAVEIN m_waveIn;
		WAVEINCAPS m_waveInCaps;
		WAVEHDR m_waveInHdr[EE_RECORDER_FRAME_NUM];
		std::vector<std::string> m_recordData;
	};
}


#endif