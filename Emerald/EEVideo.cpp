#include "EEVideo.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEVideo
	//----------------------------------------------------------------------------------------------------
	bool EEVideo::s_isVideoInitialized = false;

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::InitializeVideo()
	{
		if (!s_isVideoInitialized)
		{
			av_register_all();

			s_isVideoInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEVideo::EEVideo()
		:
		m_formatContext(nullptr),
		m_streamIndex(-1),
		m_codecContext(nullptr),
		m_swsContext(nullptr)
	{
		InitializeVideo();

	}

	//----------------------------------------------------------------------------------------------------
	EEVideo::~EEVideo()
	{
		for (auto& packet : m_data)
		{
			avpicture_free((AVPicture*)packet);
			av_free_packet(packet);
			delete packet;
			packet = nullptr;
		}
		m_data.clear();
		if (m_frameRGBA)
		{
			av_frame_free(&m_frameRGBA);
		}
		if (m_codecContext)
		{
			avcodec_close(m_codecContext);
		}
		if (m_formatContext)
		{
			avformat_close_input(&m_formatContext);
		}
	}

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::Open(const char* _fileName)
	{
		return LoadVideo(_fileName);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::Play()
	{

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::LoadVideo(const char* _fileName)
	{
		AVCodec *codec = NULL;

		// open file
		if (avformat_open_input(&m_formatContext, _fileName, NULL, NULL) < 0)
		{
			return false;
		}

		// find stream info
		if (avformat_find_stream_info(m_formatContext, NULL) < 0)
		{
			//unable to find stream info
			avformat_close_input(&m_formatContext);
			return false;
		}
		// find the stream
		if ((m_streamIndex = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, 0, 0, NULL, 0)) < 0)
		{
			avformat_close_input(&m_formatContext);
			return false;
		}

		// find decoder
		m_codecContext = m_formatContext->streams[m_streamIndex]->codec;
		codec = avcodec_find_decoder(m_codecContext->codec_id);
		if (!codec)
		{
			avformat_close_input(&m_formatContext);
			return false;
		}
		// open codec
		if (avcodec_open2(m_codecContext, codec, NULL) < 0)
		{
			avformat_close_input(&m_formatContext);
			return false;
		}

		m_width = m_codecContext->width;
		m_height = m_codecContext->height;
		m_swsContext = sws_getContext(m_codecContext->width, m_codecContext->height, m_codecContext->pix_fmt, m_width, m_height, AV_PIX_FMT_RGBA, SWS_BICUBIC, nullptr, nullptr, nullptr);
		if (!m_swsContext)
		{
			avformat_close_input(&m_formatContext);
			return false;
		}
		m_frameRGBA = av_frame_alloc();
		avpicture_alloc((AVPicture*)m_frameRGBA, AV_PIX_FMT_RGBA, m_width, m_height);
		m_frameRate = m_codecContext->framerate.num / m_codecContext->framerate.den;
		m_totalTime = m_formatContext->duration / (double)AV_TIME_BASE;
		int count = m_frameRate * m_totalTime * 1.1;
		m_data.reserve(count);

		int flag = 0;
		while (flag >= 0)
		{
			AVPacket *packet = new AVPacket;
			av_init_packet(packet);

			flag = av_read_frame(m_formatContext, packet);
			if (flag >= 0)
			{
				if (packet->stream_index == m_streamIndex)
				{
					m_data.push_back(packet);
				}
				else
				{
					av_free_packet(packet);
					delete packet;
					packet = nullptr;
				}
			}
			else
			{
				av_free_packet(packet);
				delete packet;
				packet = nullptr;
			}
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	int EEVideo::GetPacketSize()
	{
		return m_data.size();
	}

	//----------------------------------------------------------------------------------------------------
	EETexture EEVideo::GetFrame(int _num)
	{
		if (_num < GetPacketSize())
		{
			AVFrame	*frame = av_frame_alloc();

			int got = 0;
			if (avcodec_decode_video2(m_codecContext, frame, &got, m_data[_num]) < 0)
			{
				printf("Error in decoding video frame.\n");
			}
			if (got > 0)
			{
				if (m_swsContext)
				{
					sws_scale(m_swsContext, frame->data, frame->linesize, 0, m_codecContext->height, m_frameRGBA->data, m_frameRGBA->linesize);
					
					av_frame_free(&frame);
					return EETexture((unsigned char*)m_frameRGBA->data[0], m_width, m_height);
				}
			}

			av_frame_free(&frame);
		}

		return EETexture();
	}
}