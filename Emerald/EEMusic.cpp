#include "EEMusic.h"
#include "EEHelper.h"

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


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEMusicCallBack
	//----------------------------------------------------------------------------------------------------
	COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE EEMusicCallBack::OnBufferEnd(void* _context)
	{
		if (!music->SubmitBuffer())
			music->m_state = EE_MUSIC_NO_BUFFER;
		//printf("0\n");
	}

	COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE EEMusicCallBack::OnBufferStart(void* _context)
	{
		//printf("1\n");
	}

	COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE EEMusicCallBack::OnVoiceProcessingPassEnd()
	{
		//printf("2\n");
	}

	COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE EEMusicCallBack::OnVoiceProcessingPassStart(UINT32 _samplesRequired)
	{
		//printf("3 %d\n", _samplesRequired);
	}

	COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE EEMusicCallBack::OnStreamEnd()
	{
		//printf("4\n");
	}
	COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE EEMusicCallBack::OnLoopEnd(void* _context)
	{
		//printf("5\n");
	}

	COM_DECLSPEC_NOTHROW void STDMETHODCALLTYPE EEMusicCallBack::OnVoiceError(void* _context, HRESULT _error)
	{
		//printf("6");
	}


	// EEMusic
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
				// CoUninitialize();
				return false;
			}
			if (FAILED(s_XAudio2->CreateMasteringVoice(&s_masteringVoice)))
			{
				MessageBoxW(NULL, L"Create mastering voice failed!", L"ERROR", MB_OK);
				s_XAudio2->Release();
				s_XAudio2 = NULL;
				// CoUninitialize();
				return false;
			}

			s_isMusicInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic()
		:
		m_musicCallBack(this),
		m_totalBytes(0),
		m_totalSamples(0),
		m_totalTime(0.f),
		m_beginSamples(0),
		m_state(EE_MUSIC_DEFAULT)
	{
		InitializeMusic();
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic(const WAVEFORMATEX& _format)
		:
		m_musicCallBack(this),
		m_format(_format),
		m_totalBytes(0),
		m_totalSamples(0),
		m_totalTime(0.f),
		m_beginSamples(0),
		m_state(EE_MUSIC_DEFAULT)
	{
		if (InitializeMusic())
			s_XAudio2->CreateSourceVoice(&m_sourceVoice, &m_format);
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic(const char* _fileName)
		:
		m_musicCallBack(this),
		m_totalBytes(0),
		m_totalSamples(0),
		m_totalTime(0.f),
		m_beginSamples(0),
		m_state(EE_MUSIC_DEFAULT)
	{
		if (InitializeMusic())
		{
			InitializeMusic();

			Open(_fileName);
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic(const std::string& _fileName)
		:
		m_musicCallBack(this),
		m_totalBytes(0),
		m_totalSamples(0),
		m_totalTime(0.f),
		m_beginSamples(0),
		m_state(EE_MUSIC_DEFAULT)
	{
		if (InitializeMusic())
		{
			InitializeMusic();

			Open(_fileName.c_str());
		}
	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::EEMusic(const EEMusic& _musformatContext)
		:
		m_musicCallBack(this)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EEMusic::~EEMusic()
	{
		if (m_loader)
		{
			m_loader->interrupt();
			m_loader->join();
			SAFE_DELETE(m_loader);
		}
		if (m_sourceVoice)
			m_sourceVoice->DestroyVoice();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Open(const char* _fileName)
	{
		if (!SyncLoadMusic(_fileName))
			return false;

		return true;
	}

	bool EEMusic::Open(const std::string& _fileName)
	{
		return Open(_fileName.c_str());
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Start()
	{
		if (FAILED(m_sourceVoice->Start(0, XAUDIO2_COMMIT_NOW)))
			return false;

		m_state = EE_MUSIC_PLAYING;
		if (!SubmitBuffer())
			m_state = EE_MUSIC_NO_BUFFER;
		else
		{
			for (int i = 1; i < XAUDIO2_MAX_QUEUED_BUFFERS; ++i)
			{
				SubmitBuffer();
			}
		}

		return true;
	}

	// _begin: 0.f-1.f
	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Play(float _begin)
	{
		if (FAILED(m_sourceVoice->FlushSourceBuffers()))
			return false;

		m_beginSamples = (int)(_begin * m_totalSamples);
		unsigned int beginBytes = m_beginSamples * (m_format.wBitsPerSample / 8);
		for (auto& data : m_data)
		{
			if (data.second.size() <= beginBytes)
			{
				beginBytes -= data.second.size();
			}
			else
			{
				PushBuffer(EEMusicCell(&data, beginBytes));
				beginBytes = 0;
			}
		}
		// todo
		// PushBuffer(EEMusicCell(nullptr, 0, 0, EE_MUSIC_CELL_END));

		return Start();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Play(float _begin, float _len, int _times)
	{
		if (FAILED(m_sourceVoice->FlushSourceBuffers()))
			return false;

		m_beginSamples = (int)(_begin * m_totalSamples);
		for (int times = 0; times < _times; ++times)
		{
			unsigned int beginBytes = m_beginSamples * (m_format.wBitsPerSample / 8);
			unsigned int byteLen = (int)(_len * m_totalSamples) * (m_format.wBitsPerSample / 8);
			for (auto& data : m_data)
			{
				if (data.second.size() <= beginBytes)
				{
					beginBytes -= data.second.size();
				}
				else if (data.second.size() - beginBytes <= byteLen)
				{
					XAUDIO2_BUFFER buffer;
					ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
					if (times != _times - 1)
						PushBuffer(EEMusicCell(&data, beginBytes, 0, EE_MUSIC_CELL_DEFAULT));
					else
						PushBuffer(EEMusicCell(&data, beginBytes, 0, EE_MUSIC_CELL_END));
					byteLen -= data.second.size() - beginBytes;
					beginBytes = 0;
				}
				else if (byteLen != 0) // data.size() - beginBytes > byteLen && byteLen != 0
				{
					XAUDIO2_BUFFER buffer;
					ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
					if (times != _times - 1)
						PushBuffer(EEMusicCell(&data, beginBytes, byteLen, EE_MUSIC_CELL_DEFAULT));
					else
						PushBuffer(EEMusicCell(&data, beginBytes, byteLen, EE_MUSIC_CELL_END));
					byteLen = 0;
					beginBytes = 0;
					break;
				}
				else
					break;
			}
		}

		return Start();
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Pause()
	{
		if (FAILED(m_sourceVoice->Stop()))
			return false;

		m_state = EE_MUSIC_PAUSE;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::Stop()
	{
		if (FAILED(m_sourceVoice->FlushSourceBuffers()))
			return false;
		if (FAILED(m_sourceVoice->Stop()))
			return false;

		m_state = EE_MUSIC_DEFAULT;

		return true;
	}

	// todo
	//----------------------------------------------------------------------------------------------------
	bool EEMusic::AddBuffer(const char* _buffer, unsigned int _size)
	{
		if (m_data.empty())
			m_data.push_back(std::pair<int, std::string>(0, std::string(_buffer, _size)));
		else
			m_data.push_back(std::pair<int, std::string>(m_data.back().first + m_data.back().second.size(), std::string(_buffer, _size)));
		m_totalBytes += _size;
		m_totalSamples += _size / m_format.nBlockAlign;
		m_totalTime += (double)_size / m_format.nBlockAlign / m_format.nSamplesPerSec;

		XAUDIO2_BUFFER buffer;
		ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
		buffer.Flags = 0;
		buffer.AudioBytes = _size;
		buffer.pAudioData = (BYTE*)&(m_data.back().second[0]);
		buffer.PlayBegin = 0;
		buffer.PlayLength = 0;
		buffer.LoopBegin = 0;
		buffer.LoopLength = 0;
		buffer.LoopCount = 0;
		buffer.pContext = (void*)&m_data.back();
		if (FAILED(m_sourceVoice->SubmitSourceBuffer(&buffer)))
			return false;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::AddBuffer(const std::string& _buffer)
	{
		return AddBuffer(_buffer.c_str(), _buffer.size());
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::SyncLoadMusic(const char* _fileName)
	{
		AVFormatContext *formatContext = NULL;
		int streamIndex = -1;
		AVCodecContext *codecContext = NULL;
		AVCodec *codec = NULL;

		// open file
		if (avformat_open_input(&formatContext, _fileName, NULL, NULL) < 0)
		{
			return false;
		}

		// find stream info
		if (avformat_find_stream_info(formatContext, NULL) < 0)
		{
			//unable to find stream info
			avformat_close_input(&formatContext);
			return false;
		}
		// find the stream
		if ((streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, 0, 0, NULL, 0)) < 0)
		{
			avformat_close_input(&formatContext);
			return false;
		}

		// find decoder
		codecContext = formatContext->streams[streamIndex]->codec;
		codec = avcodec_find_decoder(codecContext->codec_id);
		if (!codec)
		{
			avformat_close_input(&formatContext);
			return false;
		}
		// open codec
		if (avcodec_open2(codecContext, codec, NULL) < 0)
		{
			avformat_close_input(&formatContext);
			return false;
		}

		int channels = codecContext->channels;
		int bitsPerSample = av_get_bits_per_sample_fmt(codecContext->sample_fmt);
		int bytesPerSample = bitsPerSample / 8;
		int samplesPerSec = codecContext->sample_rate;
		int blockAlign = bytesPerSample * channels;
		int avgBytesPerSec = samplesPerSec * blockAlign;
		//m_totalBytes = (int)((double)formatContext->duration / AV_TIME_BASE * avgBytesPerSec);
		//m_totalSamples = (int)((double)formatContext->duration / AV_TIME_BASE * samplesPerSec);
		//m_totalTime = formatContext->duration / (double)AV_TIME_BASE;

		/*
		The WAVEFORMATEX structure can describe only a subset of the formats that can be described by the WAVEFORMATEXTENSIBLE structure. For example, WAVEFORMATEX can describe mono or (two-channel) stereo pulse-code modulated (PCM) streams with 8-bit or 16-bit integer sample values, or with 32-bit floating-point sample values. In addition, WAVEFORMATEX can describe popular non-PCM formats such as AC-3 and WMA Pro.
		*/
		if (bitsPerSample == 32)
			m_format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;											/* format type */
		else
			m_format.wFormatTag = WAVE_FORMAT_PCM;
		m_format.nChannels = channels;																/* number of channels (i.e. mono, stereo...) */
		m_format.nSamplesPerSec = samplesPerSec;													/* sample rate */
		m_format.nAvgBytesPerSec = avgBytesPerSec;													/* for buffer estimation */
		m_format.nBlockAlign = blockAlign;															/* block size of data */
		m_format.wBitsPerSample = bitsPerSample;													/* number of bits per sample of mono data */
		m_format.cbSize = 0;																		/* extra information (after cbSize) */
		if (FAILED(s_XAudio2->CreateSourceVoice(&m_sourceVoice, &m_format)))
			return false;

		if (m_data.empty())
			m_data.push_back(std::pair<int, std::string>(0, std::string()));
		else
			m_data.push_back(std::pair<int, std::string>(m_data.back().first + m_data.back().second.size(), std::string()));
		std::string& data = m_data.back().second;
		data.resize((int)((double)formatContext->duration / AV_TIME_BASE * avgBytesPerSec) + avgBytesPerSec);

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
					printf("Error in decoding audio frame.\n");
					av_free_packet(packet);
					continue;
				}
				if (got > 0)
				{
					int size = frame->nb_samples * bytesPerSample;
					if (av_sample_fmt_is_planar(codecContext->sample_fmt))
					{
						for (int i = 0; i < size; i += bytesPerSample)
						{
							for (int j = 0; j < bytesPerSample; ++j)
							{
								data[len++] = (char)frame->data[0][i + j];
							}
							for (int j = 0; j < bytesPerSample; ++j)
							{
								data[len++] = (char)frame->data[1][i + j];
							}
						}
					}
					else
					{
						memcpy((&data[0]) + len, frame->data[0], size);
						len += size;
					}
				}
			}
			av_free_packet(packet);
		}
		m_totalBytes = len;
		m_totalSamples = len / blockAlign;
		m_totalTime = (double)m_totalSamples / samplesPerSec;
		data.resize(m_totalBytes);

		avcodec_close(codecContext);
		avformat_close_input(&formatContext);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::SyncLoadMusic(const std::string& _fileName)
	{
		return SyncLoadMusic(_fileName.c_str());
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::AsyncLoadMusic(const char* _fileName)
	{
		AVFormatContext *formatContext = NULL;
		int streamIndex = -1;
		AVCodecContext *codecContext = NULL;
		AVCodec *codec = NULL;

		// open file
		if (avformat_open_input(&formatContext, _fileName, NULL, NULL) < 0)
		{
			return false;
		}

		// find stream info
		if (avformat_find_stream_info(formatContext, NULL) < 0)
		{
			//unable to find stream info
			avformat_close_input(&formatContext);
			return false;
		}
		// find the stream
		if ((streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_AUDIO, 0, 0, NULL, 0)) < 0)
		{
			avformat_close_input(&formatContext);
			return false;
		}

		// find decoder
		codecContext = formatContext->streams[streamIndex]->codec;
		codec = avcodec_find_decoder(codecContext->codec_id);
		if (!codec)
		{
			avformat_close_input(&formatContext);
			return false;
		}
		// open codec
		if (avcodec_open2(codecContext, codec, NULL) < 0)
		{
			avformat_close_input(&formatContext);
			return false;
		}

		int channels = codecContext->channels;
		int bitsPerSample = av_get_bits_per_sample_fmt(codecContext->sample_fmt);
		int bytesPerSample = bitsPerSample / 8;
		int samplesPerSec = codecContext->sample_rate;
		int blockAlign = bytesPerSample * channels;
		int avgBytesPerSec = samplesPerSec * blockAlign;
		// m_totalBytes = (int)((double)formatContext->duration / AV_TIME_BASE * avgBytesPerSec);
		// m_totalSamples = (int)((double)formatContext->duration / AV_TIME_BASE * samplesPerSec);
		// m_totalTime = formatContext->duration / (double)AV_TIME_BASE;

		if (bitsPerSample == 32)
			m_format.wFormatTag = WAVE_FORMAT_IEEE_FLOAT;
		else
			m_format.wFormatTag = WAVE_FORMAT_PCM;
		m_format.nChannels = channels;
		m_format.nSamplesPerSec = samplesPerSec;
		m_format.nAvgBytesPerSec = avgBytesPerSec;
		m_format.nBlockAlign = blockAlign;
		m_format.wBitsPerSample = bitsPerSample;
		m_format.cbSize = 0;
		if (FAILED(s_XAudio2->CreateSourceVoice(&m_sourceVoice, &m_format, 0, XAUDIO2_DEFAULT_FREQ_RATIO, &m_musicCallBack)))
			return false;

		// todo: keep the the handler of the thr
		m_loader = new boost::thread([&, bytesPerSample, formatContext, streamIndex, codecContext, codec] () mutable ->bool {
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
						printf("Error in decoding audio frame.\n");
						av_free_packet(packet);
						continue;
					}
					if (got > 0)
					{
						//int size = *frame->linesize;
						int size = frame->nb_samples * bytesPerSample;
						if (m_data.empty())
							m_data.push_back(std::pair<int, std::string>(0, std::string()));
						else
							m_data.push_back(std::pair<int, std::string>(m_data.back().first + m_data.back().second.size(), std::string()));
						std::string& data = m_data.back().second;
						if (av_sample_fmt_is_planar(codecContext->sample_fmt))
						{
							data.resize(size * 2);
							int index = 0;
							for (int i = 0; i < size; i += bytesPerSample)
							{
								for (int j = 0; j < bytesPerSample; ++j)
								{
									data[index++] = (char)frame->data[0][i + j];
								}
								for (int j = 0; j < bytesPerSample; ++j)
								{
									data[index++] = (char)frame->data[1][i + j];
								}
							}
							len += size * 2;
						}
						else
						{
							data.resize(size);
							memcpy((&data[0]), frame->data[0], size);
							len += size;
						}
						try
						{
							PushBuffer(EEMusicCell(&m_data.back()));
							if (EE_MUSIC_NO_BUFFER == m_state)
							{
								SubmitBuffer();
								SubmitBuffer();
								SubmitBuffer();
							}
							EEThreadSleep(1);
						}
						catch (boost::thread_interrupted&)
						{
							avcodec_close(codecContext);
							avformat_close_input(&formatContext);
							return false;
						}
					}
				}
				av_free_packet(packet);
			}
			m_totalBytes = len;
			m_totalSamples = len / m_format.nBlockAlign;
			m_totalTime = (double)m_totalSamples / m_format.nSamplesPerSec;

			avcodec_close(codecContext);
			avformat_close_input(&formatContext);

			return true;
		});

		return true;
	}

	bool EEMusic::AsyncLoadMusic(const std::string& _fileName)
	{
		return AsyncLoadMusic(_fileName.c_str());
	}

	// 0.0-1.0-N
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
	const WAVEFORMATEX& EEMusic::GetFormat()
	{
		return m_format;
	}

	//----------------------------------------------------------------------------------------------------
	int EEMusic::GetBitsPerSample()
	{
		return m_format.wBitsPerSample;
	}

	//----------------------------------------------------------------------------------------------------
	int EEMusic::GetChannels()
	{
		return m_format.nChannels;
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
		if (m_sourceVoice)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);

			return (int)state.SamplesPlayed + m_beginSamples;
		}

		return -1;
	}

	//----------------------------------------------------------------------------------------------------
	double EEMusic::GetTotalTime()
	{
		return m_totalTime;
	}

	//----------------------------------------------------------------------------------------------------
	float EEMusic::GetProgress()
	{
		return (float)GetSampled() / GetTotalSamples();
	}

	//----------------------------------------------------------------------------------------------------
	double EEMusic::GetProgressTime()
	{
		return (double)GetSampled() / m_format.nSamplesPerSec;
	}

	//----------------------------------------------------------------------------------------------------
	std::string EEMusic::GetCurrentSamples(int _count)
	{
		if (m_sourceVoice)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);

			if (state.pCurrentBufferContext)
			{
				auto tmp = (std::pair<int, std::string>*)state.pCurrentBufferContext;
				// std::cout << tmp->first / m_format.nBlockAlign << "  " << (int)state.SamplesPlayed + m_beginSamples << "  " <<  << std::endl;
				// std::cout << (int)((state.SamplesPlayed + m_beginSamples) * m_format.nBlockAlign - tmp->first) << std::endl;
				int pos = (int)((state.SamplesPlayed + m_beginSamples) * m_format.nBlockAlign - tmp->first);
				if (pos < 0)
				{ 
					// error
				}
				else
				{
					return tmp->second.substr(pos, m_format.nBlockAlign * _count);
				}
			}
		}

		return std::string();
	}
	
	//----------------------------------------------------------------------------------------------------
	std::string EEMusic::GetCurrentLeftSamples(int _count)
	{
		std::string result;

		if (m_sourceVoice)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);

			if (state.pCurrentBufferContext)
			{
				auto tmp = (std::pair<int, std::string>*)state.pCurrentBufferContext;
				int pos = (int)((state.SamplesPlayed + m_beginSamples) * m_format.nBlockAlign - tmp->first);
				if (pos < 0)
				{
					// error
				}
				else
				{
					int bytes = m_format.wBitsPerSample >> 3;
					unsigned int endPos = (unsigned int)(pos + _count * m_format.nBlockAlign > tmp->second.size()) ? tmp->second.size() : pos + _count * m_format.nBlockAlign;
					for (unsigned int i = pos; i < endPos; i += m_format.nBlockAlign)
					{
						result += tmp->second.substr(i, bytes);
					}
				}
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<double> EEMusic::GetCurrentLeftSamplesDouble(int _count)
	{
		std::vector<double> result;

		if (m_sourceVoice)
		{
			XAUDIO2_VOICE_STATE state;
			m_sourceVoice->GetState(&state);

			if (state.pCurrentBufferContext)
			{
				auto tmp = (std::pair<int, std::string>*)state.pCurrentBufferContext;
				int pos = (int)((state.SamplesPlayed + m_beginSamples) * m_format.nBlockAlign - tmp->first);
				if (pos < 0)
				{
					// error
				}
				else
				{
					int bytes = m_format.wBitsPerSample >> 3;
					unsigned int endPos = pos + _count * m_format.nBlockAlign > tmp->second.size() ? tmp->second.size() : pos + _count * m_format.nBlockAlign;
					for (unsigned int i = pos; i < endPos; i += m_format.nBlockAlign)
					{
						double value = 0;
						// Big Endian
						if (EEIsBigEndian())
						{
							for (int k = 0; k < (bytes > 4 ? 4 : bytes); ++k)
							{
								value *= (int)1 << 8;
								value += tmp->second[i + k];
							}
						}
						// Little Endian
						else
						{
							for (int k = 0; k < (bytes > 4 ? 4 : bytes); ++k)
							{
								value *= (int)1 << 8;
								value += tmp->second[i + bytes - 1 - k];
							}
						}
						result.push_back(value);
					}
				}
			}
		}

		return result;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::PushBuffer(const EEMusicCell& _buffer)
	{
		m_playListMutex.lock();

		m_playList.push(_buffer);

		m_playListMutex.unlock();
		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEMusic::SubmitBuffer()
	{
		m_playListMutex.lock();

		if (!m_playList.empty())
		{
			EEMusicCell &cell = m_playList.front();
			if (cell.data)
			{
				XAUDIO2_BUFFER buffer;
				ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
				if (EE_MUSIC_CELL_DEFAULT == cell.flag)
					buffer.Flags = 0;
				else if (EE_MUSIC_CELL_END == cell.flag)
					buffer.Flags = XAUDIO2_END_OF_STREAM;
				buffer.AudioBytes = cell.data->second.size();
				buffer.pAudioData = (BYTE*)&(cell.data->second)[0];
				buffer.PlayBegin = cell.begin / (m_format.wBitsPerSample / 8);
				buffer.PlayLength = cell.playLength / (m_format.wBitsPerSample / 8);
				buffer.LoopBegin = 0;
				buffer.LoopLength = 0;
				buffer.LoopCount = 0;
				buffer.pContext = (void*)&(*cell.data);
				if (FAILED(m_sourceVoice->SubmitSourceBuffer(&buffer)))
				{
					m_playListMutex.unlock();
					return false;
				}
				else
				{
					if (EE_MUSIC_NO_BUFFER == m_state)
						m_state = EE_MUSIC_PLAYING;
					m_playList.pop();

					m_playListMutex.unlock();
					return true;
				}
			}
		}

		m_playListMutex.unlock();
		return false;
	}
}