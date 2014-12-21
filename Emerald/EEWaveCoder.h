#pragma once
#ifndef _EE_WAVECODER_H_
#define _EE_WAVECODER_H_

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	class EEWaveCoder
	{
	protected:
		static bool InitializeWaveCoder();

	protected:
		static bool s_isWaveCoderInitialized;

	public:
		EEWaveCoder();
		~EEWaveCoder();

		static bool WaveEncode(char *_dataIn, int _inLen, char *_dataOut, int *_outLen);
		static bool WaveDecode(char *_dataIn, int _inLen, char *_dataOut, int *_outLen);
	};
}

#endif