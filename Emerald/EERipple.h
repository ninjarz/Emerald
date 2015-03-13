#pragma once
#ifndef _EE_RIPPLE_H_
#define _EE_RIPPLE_H_

#include "EETexture.h"
#include "EEEffect.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	/*
	//EERipple
	//----------------------------------------------------------------------------------------------------
	class EERipple : public EEEffect
	{
	public:


	protected:

	};
	*/

	//EEDisturbBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EEDisturbBufferDesc
	{
		INT2 pos;
		int range;
		int weight;
	};

	//EESpreadBufferDesc
	//----------------------------------------------------------------------------------------------------
	struct EESpreadBufferDesc
	{
		int spreadFactor;
		int fadeFactor;
		float tmp1;
		float tmp2;
	};

	//EERippleC
	//----------------------------------------------------------------------------------------------------
	class EERippleC : public EEEffect
	{
	protected:
		static bool InitializeRippleC();

	protected:
		static bool s_isRippleCInitialized;
		static ID3D11Buffer *s_disturbBuffer;
		static ID3D11Buffer *s_spreadBuffer;
		static ID3D11ComputeShader *s_disturbCS;
		static ID3D11ComputeShader *s_spreadCS;
		static ID3D11ComputeShader *s_rippleCS;

	public:
		EERippleC(EETexture& _target);

		virtual bool Update();
		virtual bool Render();
		bool Disturb(int _x, int _y, int _range, int _weight);

		bool SetSpreadFactor(int _factor);
		bool SetFadeFactor(int _factor);

	protected:
		EETexture m_target;
		EETexture m_backup;
		EETexture m_weightMap[2];
		unsigned int m_currWeightMap;
		int m_spreadFactor;
		int m_fadeFactor;
	};

}


#endif