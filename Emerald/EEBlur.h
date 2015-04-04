#pragma once
#ifndef _EE_BLUR_H_
#define _EE_BLUR_H_

#include "EETexture.h"
#include "EEEffect.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBlurC (Gaussian blur)
	//----------------------------------------------------------------------------------------------------
	class EEBlurC : public EEEffect
	{
	public:
		EEBlurC(EETexture& _target);


	protected:
		EETexture m_target;
		EETexture m_backup;
	};

}


#endif