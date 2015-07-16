#include "EEIntBoard.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEIntBoard
	//----------------------------------------------------------------------------------------------------
	EEIntBoard::EEIntBoard(const Rect_Float& _boardRect, int _range, EETexture* _textures)
		:
		EEQuad2D(_boardRect),
		m_range(_range),
		m_isRangeDirty(true),
		m_value(0),
		m_isValueDirty(true),
		m_num(_range)
	{
		for (int i = 0; i < 10; ++i)
			m_numTexs[i] = _textures[i];
	}

	//----------------------------------------------------------------------------------------------------
	EEIntBoard::EEIntBoard(const EEIntBoard& _intBoard)
		:
		EEQuad2D(_intBoard),
		m_range(_intBoard.m_range),
		m_isRangeDirty(_intBoard.m_isRangeDirty),
		m_value(_intBoard.m_value),
		m_isValueDirty(_intBoard.m_isValueDirty),
		m_num(_intBoard.m_num)
	{
		for (int i = 0; i < 10; ++i)
			m_numTexs[i] = _intBoard.m_numTexs[i];
	}

	//----------------------------------------------------------------------------------------------------
	EEIntBoard::~EEIntBoard()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEIntBoard::Update()
	{
		if (!EEQuad2D::Update())
			return false;

		if (m_isRangeDirty)
		{
			m_num.clear();
			m_num.resize(m_range);

			int width = (int)(m_quadWidth / m_range); //should not be zero
			for (int i = 0; i < m_range; ++i)
			{
				m_num[i].SetParent(this);
				m_num[i].SetLocalZOrder(m_localZOrder); //It is wrong~~
				m_num[i].SetRect(Rect_Float((float)(m_range - i - 1) * width, 0.0f, (float)(m_range - i) * width, (float)m_quadHeight) - FLOAT2(GetWidht() / 2, GetHeight() / 2));
				m_num[i].SetIsUseColor(false);
				m_num[i].SetIsUseTex(true);
			}

			m_isRangeDirty = false;
		}

		if (m_isValueDirty)
		{
			int value = m_value >= 0 ? m_value : -m_value;
			for (int i = 0; i < m_range; ++i)
			{
				m_num[i].SetTexture(m_numTexs[value % 10]);
				value /= 10;
			}

			m_isValueDirty = false;
		}

		for (int i = 0; i < m_range; ++i)
		{
			m_num[i].Update();
		}


		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEIntBoard::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		for (int i = 0; i < m_range; ++i)
		{
			m_num[i].Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEIntBoard::SetValue(int _value)
	{
		m_value = _value;
		m_isValueDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEIntBoard::SetRange(int _range)
	{
		m_range = _range;
		m_isRangeDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	int EEIntBoard::GetValue()
	{
		return m_value;
	}

	//----------------------------------------------------------------------------------------------------
	int EEIntBoard::GetRange()
	{
		return m_range;
	}

}