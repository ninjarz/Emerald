#include "EEWaveCoder.h"

//contradict with ...
#pragma comment(lib, "Lib/G729aR.lib")
#include <Windows.h>
#include "Include/G729a.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//----------------------------------------------------------------------------------------------------
	bool EEWaveCoder::s_isWaveCoderInitialized = false;

	//----------------------------------------------------------------------------------------------------
	bool EEWaveCoder::InitializeWaveCoder()
	{
		if (!s_isWaveCoderInitialized)
		{
			va_g729a_init_encoder();
			va_g729a_init_decoder();

			s_isWaveCoderInitialized = true;
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	EEWaveCoder::EEWaveCoder()
	{
		InitializeWaveCoder();
	}

	//----------------------------------------------------------------------------------------------------
	EEWaveCoder::~EEWaveCoder()
	{
	}

	//----------------------------------------------------------------------------------------------------
	bool EEWaveCoder::WaveEncode(char *_dataIn, int _inLen, char *_dataOut, int *_outLen)
	{
		InitializeWaveCoder();

		if (!_dataIn || _inLen != 960 || !_dataOut)
			return false;

		va_g729a_encoder((short*)_dataIn, (BYTE*)_dataOut);
		va_g729a_encoder((short*)(_dataIn + 160), (BYTE*)_dataOut + 10);
		va_g729a_encoder((short*)(_dataIn + 320), (BYTE*)_dataOut + 20);
		va_g729a_encoder((short*)(_dataIn + 480), (BYTE*)_dataOut + 30);
		va_g729a_encoder((short*)(_dataIn + 640), (BYTE*)_dataOut + 40);
		va_g729a_encoder((short*)(_dataIn + 800), (BYTE*)_dataOut + 50);

		if (_outLen)
			*_outLen = 60;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEWaveCoder::WaveDecode(char *_dataIn, int _inLen, char *_dataOut, int *_outLen)
	{
		InitializeWaveCoder();

		if (!_dataIn || _inLen != 60 || !_dataOut)
			return false;

		va_g729a_decoder((BYTE*)_dataIn, (short*)(_dataOut), 0);
		va_g729a_decoder((BYTE*)_dataIn + 10, (short*)(_dataOut + 160), 0);
		va_g729a_decoder((BYTE*)_dataIn + 20, (short*)(_dataOut + 320), 0);
		va_g729a_decoder((BYTE*)_dataIn + 30, (short*)(_dataOut + 480), 0);
		va_g729a_decoder((BYTE*)_dataIn + 40, (short*)(_dataOut + 640), 0);
		va_g729a_decoder((BYTE*)_dataIn + 50, (short*)(_dataOut + 800), 0);

		if (_outLen)
			*_outLen = 960;

		return true;
	}
}