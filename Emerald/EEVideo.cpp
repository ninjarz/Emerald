#include "EEVideo.h"
#include "EETimer.h"


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
		m_swsContext(nullptr),
		m_width(-1),
		m_height(-1),
		m_isRunning(false),
		m_startTime(-1),
		m_currentPacket(-1)
	{
		InitializeVideo();

	}

	//----------------------------------------------------------------------------------------------------
	EEVideo::EEVideo(const Rect_Float& _rect)
		:
		m_formatContext(nullptr),
		m_streamIndex(-1),
		m_codecContext(nullptr),
		m_swsContext(nullptr),
		m_width(int(_rect.z - _rect.x)),
		m_height(int(_rect.w - _rect.y)),
		m_isRunning(false),
		m_startTime(-1),
		m_currentPacket(-1),

		m_screen(_rect)
	{
		InitializeVideo();

		m_screen.SetIsUseTex(true);
		m_screen.SetIsUseColor(false);
	}

	//----------------------------------------------------------------------------------------------------
	EEVideo::~EEVideo()
	{
		for (auto& packet : m_packets)
		{
			avpicture_free((AVPicture*)packet);
			av_free_packet(packet);
			delete packet;
			packet = nullptr;
		}
		m_packets.clear();

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
	bool EEVideo::Update()
	{
		if (m_isRunning)
		{
			double deltaTime = (EEGetTotalTime() - m_startTime) / m_timeBase;

			// handle the packets
			int nextPacket = m_currentPacket + 1;  //int currentFrame = int(deltaTime * m_frameRate);
			while (nextPacket < (int)m_packets.size())
			{
				if (m_packets[nextPacket]->dts < deltaTime)
				{
					DecodePacket(nextPacket++);
				}
				else
				{
					if (m_currentPacket != nextPacket - 1)
						m_currentPacket = nextPacket - 1;
					break;
				}
			}

			// handle the frames
			while (!m_frames.empty())
			{
				if (m_frames.front().first < deltaTime)
				{
					m_screen.SetTexture(m_frames.front().second);
					m_frames.pop();
				}
			}

			// the end
			if ((int)m_packets.size() <= m_currentPacket && m_frames.empty())
			{
				m_isRunning = false;
			}

			return m_screen.Update();
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::Render()
	{
		if (m_isRunning)
		{
			return m_screen.Render();
		}

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::Process()
	{
		if (Update())
			return Render();

		return false;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::Open(const char* _fileName)
	{
		m_music.Open(_fileName);

		return LoadVideo(_fileName);
	}

	//----------------------------------------------------------------------------------------------------
	bool EEVideo::Play()
	{
		m_isRunning = true;
		m_startTime = EEGetTotalTime();
		m_currentPacket = -1;
		m_music.Play(); // X

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	int EEVideo::GetPacketSize()
	{
		return m_packets.size();
	}

	// Get packets (ordered by dts)
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
		if ((m_streamIndex = av_find_best_stream(m_formatContext, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0)) < 0)
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

		if (m_width == -1)
			m_width = m_codecContext->width;
		if (m_height == -1)
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
		m_timeBase = av_q2d(m_formatContext->streams[m_streamIndex]->time_base);
		int count = (int)(m_frameRate * m_totalTime * 1.1);	
		m_packets.reserve(count);

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
					m_packets.push_back(packet);
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
	bool EEVideo::DecodePacket(int _num)
	{
		if (_num < GetPacketSize())
		{
			int got = 0;
			AVFrame	*frame = av_frame_alloc();
			if (avcodec_decode_video2(m_codecContext, frame, &got, m_packets[_num]) < 0)
			{
				printf("Error in decoding video frame.\n");
			}

			if (got > 0)
			{
				if (m_swsContext)
				{
					sws_scale(m_swsContext, frame->data, frame->linesize, 0, m_codecContext->height, m_frameRGBA->data, m_frameRGBA->linesize);
					m_frames.push(std::pair<int64_t, EETexture>(frame->pkt_pts, EETexture((unsigned char*)m_frameRGBA->data[0], m_width, m_height)));
					
					av_frame_free(&frame);
					return true;
				}
			}
			else
			{
				// todo: handle the rest
				// printf("%d\n", _num);
			}

			av_frame_free(&frame);
		}

		return false;
	}
}