#pragma once
#ifndef _EE_CONTRASTANDBRIGHT_H_
#define _EE_CONTRASTANDBRIGHT_H_

#include "EETexture.h"
#include "EEEffect.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEContrastAndBrightBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEContrastAndBrightBufferDesc
	{
		float contrast;
		float bright;
		float tmp2;
		float tmp3;
	};

	// EEContrastAndBrightC
	//----------------------------------------------------------------------------------------------------
	class EEContrastAndBrightC : public EEEffect
	{
	protected:
		static bool InitializeContrastAndBrightC();

	protected:
		static bool s_isContrastAndBrightCInitialized;
		static ID3D11Buffer *s_contrastAndBrightBuffer;
		static ID3D11ComputeShader *s_contrastAndBrightCS;

	public:
		EEContrastAndBrightC(EETexture& _target);

		virtual bool Update();
		virtual bool Render();

		void SetContrast(float _contrast);
		void SetBright(float _bright);

	protected:
		EETexture m_backup;
		EETexture m_target;
		float m_contrast;
		float m_bright;
	};
}


#endif