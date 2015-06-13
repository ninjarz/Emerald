#pragma once
#ifndef _EE_ERODE_H_
#define _EE_ERODE_H_

#include "EETexture.h"
#include "EEEffect.h"
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEErodeBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEErodeBufferDesc
	{
		INT2 kernel;
		float tmp2;
		float tmp3;
	};

	// EEErodeC
	//----------------------------------------------------------------------------------------------------
	class EEErodeC : public EEEffect
	{
	protected:
		static bool InitializeErodeC();

	protected:
		static bool s_isErodeCInitialized;
		static ID3D11Buffer *s_erodeBuffer;
		static ID3D11ComputeShader *s_erodeCS;

	public:
		EEErodeC(EETexture& _target, EETexture& _gray, const INT2& _kernel);

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