#include "EEVideo.h"

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

		int width = codecContext->width;
		int height = codecContext->height;

		AVPacket *packet = new AVPacket;
		av_init_packet(packet);
		AVFrame	*frame = avcodec_alloc_frame();
		int got;
		while (av_read_frame(formatContext, packet) >= 0)
		{
			if (packet->stream_index == streamIndex)
			{
				if (avcodec_decode_video2(codecContext, frame, &got, packet) < 0)
				{
					printf("Error in decoding video frame.\n");
					av_free_packet(packet);
					continue;
				}
				if (got > 0)
				{
					if (codecContext->pix_fmt == AV_PIX_FMT_YUV420P)
					{

					}
					else
					{

					}
					int i = 0;
				}
			}
			av_free_packet(packet);
		}

		return true;
	}
}