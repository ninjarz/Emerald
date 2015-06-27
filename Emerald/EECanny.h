#pragma once
#ifndef _EE_CANNY_H_
#define _EE_CANNY_H_

#include "EETexture.h"
#include "EEEffect.h"
#include "EEMath.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EECannyBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EECannyBufferDesc
	{
		INT2 kernel;
		float tmp2;
		float tmp3;
	};

	// EECannyC
	//----------------------------------------------------------------------------------------------------
	class EECannyC : public EEEffect
	{
	protected:
		static bool InitializeCannyC();

	protected:
		static bool s_isCannyCInitialized;
		static ID3D11Buffer *s_cannyBuffer;
		static ID3D11ComputeShader *s_cannyCS;

	public:
		EECannyC(EETexture& _target, EETexture& _gray, const INT2& _kernel);

		virtual bool Update();
		virtual bool Render();

	protected:
		EETexture m_backup;
		EETexture m_backupGray;
		EETexture m_gradient;
		EETexture m_target;
		INT2 m_kernel;
	};
}


#endif