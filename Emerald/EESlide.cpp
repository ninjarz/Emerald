#include "EESlide.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EESlide
	//----------------------------------------------------------------------------------------------------
	EESlide::EESlide(const Rect_Float& _rect, EETexture& _texs, float _interval, bool _isLoop)
		:
		EEQuad2D(_rect, _texs),
		m_updateTime(-1.0f),
		m_changeInterval(_interval),
		m_isLoop(_isLoop)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EESlide::EESlide(const Rect_Float& _rect, EETexture* _texs, int _amount, float _interval, bool _isLoop)
		:
		EEQuad2D(_rect, EETextureCombine(_texs, _amount)),
		m_updateTime(-1.0f),
		m_changeInterval(_interval),
		m_isLoop(_isLoop)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EESlide::EESlide(const EESlide& _slide)
		:
		EEQuad2D(_slide),
		m_updateTime(_slide.m_updateTime),
		m_changeInterval(_slide.m_changeInterval),
		m_isLoop(_slide.m_isLoop)
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EESlide::Update()
	{
		if (!EEObject::Update())
			return false;

		if (m_updateTime < 0.0f)
			m_updateTime = (float)EECore::s_EECore->GetTotalTime();
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
			{
				SetCurrentSlide(times < m_quadTex.GetNumber() ? times : m_quadTex.GetNumber() - 1);
			}
		}

		return EEQuad2D::Update();
	}

	//----------------------------------------------------------------------------------------------------
	bool EESlide::SetCurrentSlide(int _num)
	{
		return SetTexIndex(_num);
	}

	//----------------------------------------------------------------------------------------------------
	void EESlide::SetInterval(float _interval)
	{
		m_changeInterval = _interval;
	}

	//----------------------------------------------------------------------------------------------------
	void EESlide::SetIsLoop(bool _isLoop)
	{
		m_isLoop = _isLoop;
	}
}