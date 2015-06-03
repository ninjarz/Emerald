#pragma once
#ifndef _EE_AVERAGE_H_
#define _EE_AVERAGE_H_

#include "EETexture.h"
#include "EEEffect.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEAverageBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEAverageBufferDesc
	{
		int value;
		float tmp1;
		float tmp2;
		float tmp3;
	};

	// EEAverage
	//----------------------------------------------------------------------------------------------------
	class EEAverageC : public EEEffect
	{
	protected:
		static bool InitializeAverageC();

	protected:
		static bool s_isAverageCInitialized;
		static ID3D11Buffer *s_averageBuffer;
		static ID3D11ComputeShader *s_averageCS;

	public:
		EEAverageC();
		EEAverageC(const EETexture& _src);

		virtual bool Update();
		virtual bool Render();

	protected:
		EETexture m_target;
		EETexture m_backup;
	};

}


#endif