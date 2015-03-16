#include "EESlide.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESlide
	//----------------------------------------------------------------------------------------------------
	EESlide::EESlide(const Rect_Float& _rect, EETexture* _texs, int _amount)
		:
		EEQuad2D(_rect, EETextureCombine(_texs, _amount))
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EESlide::SetCurrentSlide(int _num)
	{
		return SetTexIndex(_num);
	}

}