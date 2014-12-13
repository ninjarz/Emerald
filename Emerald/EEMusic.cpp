#include "EEMusic.h"
#include "EEHelper.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEMusic
	//----------------------------------------------------------------------------------------------------
	bool EEMusic::s_isMusicInitialized = false;
	IXAudio2 *EEMusic::s_XAudio2 = NULL;
	IXAudio2MasteringVoice *EEMusic::s_masteringVoice = NULL;

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::InitializeMusic()
	{
		if (!s_isMusicInitialized)
		{
			av_register_all();

			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			if (FAILED(XAudio2Create(&s_XAudio2, 0)))
			{
				MessageBoxW(NULL, L"Create XAudio2 failed!", L"ERROR", MB_OK);
				CoUninitialize();
				return false;
			}
			if (FAILED(s_XAudio2->CreateMasteringVoice(&s_masteringVoice)))
			{
				MessageBoxW(NULL, L"Create mastering voice failed!", L"ERROR", MB_OK);
				s_XAudio2->Release();
				s_XAudio2 = NULL;
				CoUninitialize();
				return false;
			}

			s_isMusicInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic()
	{
		InitializeMusic();
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic(const char* _fileName)
	{
		InitializeMusic();

		LoadMusic(_fileName);
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic(const EEMusic& _musformatContext)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::~EEMusic()
	{
		if (m_sourceVoice)
			m_sourceVoice->DestroyVoice();
		SAFE_DELETE_ARRAY(m_buffer.pAudioData);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Open(const char* _fileName)
	{
		if (!LoadMusic(_fileName))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Play(float _begin)
	{
		if (FAILED(m_sourceVoice->FlushSourceBuffers()))
			return false;

		m_beginSamples = (int)(_begin * m_totalSamples);
		ZeroMemory(&m_buffer, sizeof(XAUDIO2_BUFFER));
		m_buffer.Flags = 0;
		m_buffer.AudioBytes = m_totalBytes;
		m_buffer.pAudioData = (BYTE*)m_data;
		m_buffer.PlayBegin = m_beginSamples;
		m_buffer.PlayLength = 0;
		m_buffer.LoopBegin = 0;
		m_buffer.LoopLength = 0;
		m_buffer.LoopCount = 0;
		m_buffer.pContext = NULL;
		if (FAILED(m_sourceVoice->SubmitSourceBuffer(&m_buffer)))
			return false;

		if (FAILED(m_sourceVoice->Start(0, XAUDIO2_COMMIT_NOW)))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Play(float _begin, float _len, int _times)
	{
		if (FAILED(m_sourceVoice->FlushSourceBuffers()))
			return false;

		m_beginSamples = (int)(_begin * m_totalSamples);
		int sampleLen = (int)(_len * m_totalSamples);
		for (int i = 0; i < _times - 1; ++i)
		{
			ZeroMemory(&m_buffer, sizeof(XAUDIO2_BUFFER));
			m_buffer.Flags = XAUDIO2_END_OF_STREAM;
			m_buffer.AudioBytes = m_totalBytes;
			m_buffer.pAudioData = (BYTE*)m_data;
			m_buffer.PlayBegin = m_beginSamples;
			m_buffer.PlayLength = sampleLen;
			m_buffer.LoopBegin = 0;
			m_buffer.LoopLength = 0;
			m_buffer.LoopCount = 0;
			m_buffer.pContext = NULL;
			if (FAILED(m_sourceVoice->SubmitSourceBuffer(&m_buffer)))
				return false;
		}
		if (_times > 0)
		{
			ZeroMemory(&m_buffer, sizeof(XAUDIO2_BUFFER));
			m_buffer.Flags = 0;
			m_buffer.AudioBytes = m_totalBytes;
			m_buffer.pAudioData = (BYTE*)m_data;
			m_buffer.PlayBegin = m_beginSamples;
			m_buffer.PlayLength = sampleLen;
			m_buffer.LoopBegin = 0;
			m_buffer.LoopLength = 0;
			m_buffer.LoopCount = 0;
			m_buffer.pContext = NULL;
			if (FAILED(m_sourceVoice->SubmitSourceBuffer(&m_buffer)))
				return false;
		}

		if (FAILED(m_sourceVoice->Start(0, XAUDIO2_COMMIT_NOW)))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Pause()
	{
		if (FAILED(m_sourceVoice->Stop()))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Stop()
	{
		if (FAILED(m_sourceVoice->FlushSourceBuffers()))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::SetVolume(float _volume)
	{
		if (FAILED(m_sourceVoice->SetVolume(_volume)))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::SetSampleRate(int _rate)
	{
		if (FAILED(m_sourceVoice->SetSourceSampleRate(_rate)))
			return false;
		m_format.nSamplesPerSec = _rate;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::SetFrequencyRatio(float _para)
	{
		if (FAILED(m_sourceVoice->SetFrequencyRatio(_para)))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	float EEMusic::GetVolume()
	{
		float volume = 0;
		m_sourceVoice->GetChannelVolumes(0, &volume);

		return volume;
	}

	//----------------------------------------------------------------------------------------------------
	int EEMusic::GetSampleRate()
	{
		return m_format.nSamplesPerSec;
	}

	//----------------------------------------------------------------------------------------------------
	int EEMusic::GetTotalSamples()
	{
		return m_totalSamples;
	}

	//----------------------------------------------------------------------------------------------------
	int EEMusic::GetSampled()
	{
		XAUDIO2_VOICE_STATE state;
		m_sourceVoice->GetState(&state);

		return (int)state.SamplesPlayed + m_beginSamples;
	}

	//----------------------------------------------------------------------------------------------------
	float EEMusic::GetProgress()
	{
		return (float)GetSampled() / GetTotalSamples();
	}

	char* EEMusic::GetSampleData(int _num)
	{
		if (_num < m_totalSamples)
			return m_data + m_format.nBlockAlign * _num;
		else
			return NULL;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::LoadMusic(const char* _fileName)
	{
		AVFormatContext *formatContext = NULL;
		int streamIndex = -1;
		AVCodecContext *codecContext = NULL;
		AVCodec *codec = NULL;
		//open file
		if (avformat_open_input(&formatContext, _fileName, NULL, NULL) < 0)
		{
			return false;
		}
		//get info
		if (avformat_find_stream_info(formatContext, NULL) < 0)
		{
			//unable to find stream info
			avformat_close_input(&formatContext);
			return false;
		}
		//find the stream
		if ((streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, 0, 0, NULL, 0)) < 0)
		{
			//if could not find the audio stream
			avformat_close_input(&formatContext);
			return false;
		}
		//get a pointer to the codec context
		codecContext = formatContext->streams[streamIndex]->codec;
		//find the decoder
		codec = avcodec_find_decoder(codecContext->codec_id);
		if (!codec)
		{
			avformat_close_input(&formatContext);
			return false;
		}
		//could not open codec
		if (avcodec_open2(codecContext, codec, NULL) < 0)
		{
			avformat_close_input(&formatContext);
			return false;
		}

		int channels = codecContext->channels;
		int bitsPerSample = av_get_bits_per_sample_fmt(codecContext->sample_fmt);
		int samplesPerSec = codecContext->sample_rate;
		int blockAlign = bitsPerSample * channels / 8;
		int avgBytesPerSec = samplesPerSec * blockAlign;
		m_totalBytes = (int)((double)formatContext->duration / AV_TIME_BASE * avgBytesPerSec);
		m_totalSamples = (int)((double)formatContext->duration / AV_TIME_BASE * samplesPerSec);
		m_totalTimes = (int)(formatContext->duration / AV_TIME_BASE);
		m_data = new char[m_totalBytes];

		AVPacket *packet = new AVPacket;
		av_init_packet(packet);
		AVFrame	*frame = avcodec_alloc_frame();

		uint32_t len = 0;
		int got;
		while (av_read_frame(formatContext, packet) >= 0)
		{
			if (packet->stream_index == streamIndex)
			{
				if (avcodec_decode_audio4(codecContext, frame, &got, packet) < 0)
				{
					//printf("Error in decoding audio frame.\n");
					return false;
				}
				if (got > 0)
				{
					int size = *frame->linesize;
					if (av_sample_fmt_is_planar(codecContext->sample_fmt))
					{
						for (int i = 0; i < size; i += 2)
						{
							int pos = len + i * 2;
							m_data[pos] = (char)frame->data[0][i];
							m_data[pos + 1] = (char)frame->data[0][i + 1];
							m_data[pos + 2] = (char)frame->data[1][i];
							m_data[pos + 3] = (char)frame->data[1][i + 1];
						}
						len += *frame->linesize * 2;
					}
					else
					{
						memcpy(m_data + len, frame->data[0], *frame->linesize);
						len += *frame->linesize;
					}
				}
			}
			av_free_packet(packet);
		}
		m_totalBytes = len;
		m_totalSamples = len / blockAlign;
		m_totalTimes = m_totalSamples / samplesPerSec;
		
		m_format.wFormatTag = WAVE_FORMAT_PCM;																/* format type */
		m_format.nChannels = channels;																		/* number of channels (i.e. mono, stereo...) */
		m_format.nSamplesPerSec = samplesPerSec;															/* sample rate */
		m_format.nAvgBytesPerSec = avgBytesPerSec;															/* for buffer estimation */
		m_format.nBlockAlign = blockAlign;																	/* block size of data */
		m_format.wBitsPerSample = bitsPerSample;															/* number of bits per sample of mono data */
		m_format.cbSize = 0;																				/* extra information (after cbSize) */
		if (FAILED(s_XAudio2->CreateSourceVoice(&m_sourceVoice, &m_format)))
			return false;

		avcodec_close(codecContext);
		avformat_close_input(&formatContext);

		return true;
	}
}