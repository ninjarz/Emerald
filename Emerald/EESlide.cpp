#include "EESlide.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESlide
	//----------------------------------------------------------------------------------------------------
	EESlide::EESlide(const Rect_Float& _rect, const EETexture* _texs, int _amount)
		:
		EEQuad2D(_rect, *_texs),
		m_slides(_texs, _texs + _amount)
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EESlide::SetCurrentSlide(int _num)
	{
		if (m_slides.size())
		{
			if (_num < 0)
			{
				SetTexture(m_slides[0]);
				return false;
			}
			else if (_num < (int)m_slides.size())
			{
				SetTexture(m_slides[_num]);
				return true;
			}
			else
			{
				SetTexture(m_slides[m_slides.size() - 1]);
				return false;
			}
		}

		return false;
	}

}