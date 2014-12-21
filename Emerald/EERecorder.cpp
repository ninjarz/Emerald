#include "EERecorder.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EERecorder
	//----------------------------------------------------------------------------------------------------
	std::vector<EERecorder*> EERecorder::s_recorders;

	//----------------------------------------------------------------------------------------------------
	void CALLBACK EERecorder::RecorderInProc(HWAVEIN _waveIn, UINT _msg, DWORD_PTR _instance, DWORD_PTR _param1, DWORD_PTR _param2)
	{
		LPWAVEHDR waveInHdr = (LPWAVEHDR)_param1;
		if (WIM_DATA == _msg)
		{
			EERecorder *recorder = FindRecorder(_waveIn);
			LPWAVEHDR waveInHdr = (LPWAVEHDR)_param1;

			recorder->AddBuffer(waveInHdr->lpData, EE_RECORDER_FRAME_SIZE);

			waveInAddBuffer(_waveIn, (LPWAVEHDR)_param1, sizeof(WAVEHDR));
		}
	}

	//----------------------------------------------------------------------------------------------------
	EERecorder* EERecorder::FindRecorder(HWAVEIN _waveIn)
	{
		for (EERecorder* recorder : s_recorders)
		{
			if (recorder->m_waveIn == _waveIn)
				return recorder;
		}

		return NULL;
	}

	//----------------------------------------------------------------------------------------------------
	EERecorder::EERecorder()
		:
		EEMusic()
	{
		s_recorders.push_back(this);

		m_format.wFormatTag = WAVE_FORMAT_PCM;
		m_format.nChannels = EE_RECORDER_CHANNELS;
		m_format.nSamplesPerSec = EE_RECORDER_SAMPLE_HIGH;
		m_format.nAvgBytesPerSec = EE_RECORDER_SAMPLE_HIGH * EE_RECORDER_BIT * EE_RECORDER_CHANNELS / 8;
		m_format.nBlockAlign = EE_RECORDER_BIT * EE_RECORDER_CHANNELS / 8;
		m_format.wBitsPerSample = EE_RECORDER_BIT;
		m_format.cbSize = 0;
		s_XAudio2->CreateSourceVoice(&m_sourceVoice, &m_format);

		waveInOpen(&m_waveIn, WAVE_MAPPER, &m_format, (DWORD_PTR)RecorderInProc, 0, CALLBACK_FUNCTION);
		waveInGetDevCaps((UINT_PTR)m_waveIn, &m_waveInCaps, sizeof(WAVEINCAPS));

		//EE_RECORDER_FRAME_SIZE bits per buffer
		for (int i = 0; i < EE_RECORDER_FRAME_NUM; ++i)
		{
			m_waveInHdr[i].lpData = new char[EE_RECORDER_FRAME_SIZE];
			m_waveInHdr[i].dwBufferLength = EE_RECORDER_FRAME_SIZE;
			m_waveInHdr[i].dwBytesRecorded = 0;
			m_waveInHdr[i].dwUser = 0;
			m_waveInHdr[i].dwFlags = 0;
			m_waveInHdr[i].dwLoops = 0;
			m_waveInHdr[i].lpNext = NULL;
			m_waveInHdr[i].reserved = 0;

			waveInPrepareHeader(m_waveIn, &m_waveInHdr[i], sizeof(WAVEHDR));
			waveInAddBuffer(m_waveIn, &m_waveInHdr[i], sizeof(WAVEHDR));
		}
	}

	//----------------------------------------------------------------------------------------------------
	EERecorder::EERecorder(DWORD_PTR _callBack)
	{
		m_format.wFormatTag = WAVE_FORMAT_PCM;
		m_format.nChannels = EE_RECORDER_CHANNELS;
		m_format.nSamplesPerSec = EE_RECORDER_SAMPLE_HIGH;
		m_format.nAvgBytesPerSec = EE_RECORDER_SAMPLE_HIGH * EE_RECORDER_BIT * EE_RECORDER_CHANNELS / 8;
		m_format.nBlockAlign = EE_RECORDER_BIT * EE_RECORDER_CHANNELS / 8;
		m_format.wBitsPerSample = EE_RECORDER_BIT;
		m_format.cbSize = 0;
		s_XAudio2->CreateSourceVoice(&m_sourceVoice, &m_format);

		waveInOpen(&m_waveIn, WAVE_MAPPER, &m_format, (DWORD_PTR)_callBack, 0, CALLBACK_FUNCTION);
		waveInGetDevCaps((UINT_PTR)m_waveIn, &m_waveInCaps, sizeof(WAVEINCAPS));

		//EE_RECORDER_FRAME_SIZE bits per buffer
		for (int i = 0; i < EE_RECORDER_FRAME_NUM; ++i)
		{
			m_waveInHdr[i].lpData = new char[EE_RECORDER_FRAME_SIZE];
			m_waveInHdr[i].dwBufferLength = EE_RECORDER_FRAME_SIZE;
			m_waveInHdr[i].dwBytesRecorded = 0;
			m_waveInHdr[i].dwUser = 0;
			m_waveInHdr[i].dwFlags = 0;
			m_waveInHdr[i].dwLoops = 0;
			m_waveInHdr[i].lpNext = NULL;
			m_waveInHdr[i].reserved = 0;

			waveInPrepareHeader(m_waveIn, &m_waveInHdr[i], sizeof(WAVEHDR));
			waveInAddBuffer(m_waveIn, &m_waveInHdr[i], sizeof(WAVEHDR));
		}
	}

	//----------------------------------------------------------------------------------------------------
	EERecorder::~EERecorder()
	{

	}

	//----------------------------------------------------------------------------------------------------
	void EERecorder::Start()
	{
		waveInStart(m_waveIn);
	}
}