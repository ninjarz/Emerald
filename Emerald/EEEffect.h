#pragma once
#ifndef _EE_EFFECT_H_
#define _EE_EFFECT_H_

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEEffect
	//----------------------------------------------------------------------------------------------------
	class EEEffect
	{
	public:
		EEEffect();
		EEEffect(EEEffect& _effect);
		virtual ~EEEffect();

		virtual bool Update();
		virtual bool Render();
		virtual bool Process();

	protected:
		bool m_isAlive;

	};

	//EEEffect_APIs
	//----------------------------------------------------------------------------------------------------
	bool EEProcess(EEEffect* _effect);
}

#endif