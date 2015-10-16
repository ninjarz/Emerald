#include "EEVideo.h"

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
	{
		InitializeVideo();

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
		if ((streamIndex = av_find_best_stream(formatContext, AVMEDIA_TYPE_VIDEO, 0, 0, NULL, 0)) < 0)
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

		m_width = codecContext->width;
		m_height = codecContext->height;
		m_frameRate = codecContext->framerate.num / codecContext->framerate.den;
		m_totalTime = formatContext->duration / (double)AV_TIME_BASE;
		int count = m_frameRate * m_totalTime * 1.1;
		m_data.reserve(count);

		SwsContext *swsContext = sws_getContext(codecContext->width, codecContext->height, codecContext->pix_fmt, m_width, m_height, AV_PIX_FMT_RGBA, SWS_BICUBIC, nullptr, nullptr, nullptr);
		if (!swsContext)
		{
			avformat_close_input(&formatContext);
			return false;
		}
		AVPacket *packet = new AVPacket;
		av_init_packet(packet);
		AVFrame	*frame = av_frame_alloc();
		AVFrame	*frameRGBA = av_frame_alloc();
		uint8_t *out_buffer = new uint8_t[avpicture_get_size(AV_PIX_FMT_RGBA, m_width, m_height)];
		avpicture_fill((AVPicture*)frameRGBA, out_buffer, AV_PIX_FMT_RGBA, m_width, m_height);
		while (av_read_frame(formatContext, packet) >= 0)
		{
			if (packet->stream_index == streamIndex)
			{
				int got = 0;
				if (avcodec_decode_video2(codecContext, frame, &got, packet) < 0)
				{
					printf("Error in decoding video frame.\n");
					av_free_packet(packet);
					continue;
				}
				if (got > 0)
				{
					if (swsContext)
					{
						sws_scale(swsContext, frame->data, frame->linesize, 0, codecContext->height, frameRGBA->data, frameRGBA->linesize);
						m_data.push_back(EEBitmap(m_width, m_height, (unsigned char*)frameRGBA->data[0])); // copy copy copy X
					}
				}
			}
			av_free_packet(packet);
		}

		av_frame_free(&frame);
		av_frame_free(&frameRGBA);
		avcodec_close(codecContext);
		avformat_close_input(&formatContext);

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	std::vector<EEBitmap>& EEVideo::GetData()
	{
		return m_data;
	}
}