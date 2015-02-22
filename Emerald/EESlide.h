#pragma once
#ifndef _EE_SLIDE_H_
#define _EE_SLIDE_H_

#include <vector>
#include "EEQuad2D.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESlide
	//----------------------------------------------------------------------------------------------------
	class EESlide : public EEQuad2D
	{
	public:
		EESlide(const Rect_Float& _rect, const EETexture* _texs, int _amount);

		bool SetCurrentSlide(int _num);


	protected:
		std::vector<EETexture> m_slides;
	};
}


#endif