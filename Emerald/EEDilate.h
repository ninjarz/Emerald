#pragma once
#ifndef _EE_DILATE_H_
#define _EE_DILATE_H_

#include "EETexture.h"
#include "EEEffect.h"
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEDilateBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEDilateBufferDesc
	{
		INT2 kernel;
		float tmp2;
		float tmp3;
	};

	// EEDilateC
	//----------------------------------------------------------------------------------------------------
	class EEDilateC : public EEEffect
	{
	protected:
		static bool InitializeDilateC();

	protected:
		static bool s_isDilateCInitialized;
		static ID3D11Buffer *s_dilateBuffer;
		static ID3D11ComputeShader *s_dilateCS;

	public:
		EEDilateC(EETexture& _target, EETexture& _gray, const INT2& _kernel);

		virtual bool Update();
		virtual bool Render();

	protected:
		EETexture m_backup;
		EETexture m_backupGray;
		EETexture m_target;
		INT2 m_kernel;
	};
}


#endif