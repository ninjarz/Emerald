#include "EESlide.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESlide
	//----------------------------------------------------------------------------------------------------
	EESlide::EESlide(const Rect_Float& _rect, EETexture& _texs, float _interval, bool _isLoop)
		:
		EEQuad2D(_rect, _texs),
		m_updateTime((float)EECore::s_EECore->GetTotalTime()),
		m_changeInterval(_interval),
		m_isLoop(_isLoop)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EESlide::EESlide(const Rect_Float& _rect, EETexture* _texs, int _amount, float _interval, bool _isLoop)
		:
		EEQuad2D(_rect, EETextureCombine(_texs, _amount)),
		m_updateTime((float)EECore::s_EECore->GetTotalTime()),
		m_changeInterval(_interval),
		m_isLoop(_isLoop)
	{

	}

	bool EESlide::Update()
	{
		if (!EEObject::Update())
			return false;

		if (m_changeInterval <= 0.0f)
			m_updateTime = (float)EECore::s_EECore->GetTotalTime();
		else
		{
			int times = (int)((EECore::s_EECore->GetTotalTime() - m_updateTime) / m_changeInterval);
			m_updateTime += m_changeInterval * times;
			times += GetTexIndex();
			if (m_isLoop)
				SetCurrentSlide(times % m_quadTex.GetNumber());
			else
				SetCurrentSlide(times < m_quadTex.GetNumber() ? times : m_quadTex.GetNumber() - 1);
		}

		return EEQuad2D::Update();
	}

	//----------------------------------------------------------------------------------------------------
	bool EESlide::SetCurrentSlide(int _num)
	{
		return SetTexIndex(_num);
	}

}