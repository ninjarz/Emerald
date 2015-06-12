#pragma once
#ifndef _EE_GRAY_H_
#define _EE_GRAY_H_

#include "EETexture.h"
#include "EEEffect.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEGrayBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEGrayBufferDesc
	{
		int value;
		float tmp1;
		float tmp2;
		float tmp3;
	};

	// EEGrayC
	//----------------------------------------------------------------------------------------------------
	class EEGrayC : public EEEffect
	{
	protected:
		static bool InitializeGrayC();

	protected:
		static bool s_isGrayCInitialized;
		static ID3D11Buffer *s_grayBuffer;
		static ID3D11ComputeShader *s_grayCS;

	public:
		EEGrayC(EETexture& _target);

		virtual bool Update();
		virtual bool Render();

	protected:
		EETexture m_target;
		EETexture m_backup;
	};
}


#endif