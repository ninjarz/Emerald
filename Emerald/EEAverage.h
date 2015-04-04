#pragma once
#ifndef _EE_AVERAGE_H_
#define _EE_AVERAGE_H_

#include "EETexture.h"
#include "EEEffect.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEBlurC
	class EEAverageC : public EEEffect
	{
	public:
		EEAverageC(EETexture& _src);


	protected:
		EETexture m_backup;
	};

}


#endif