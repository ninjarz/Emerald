#include "EEFloatBoard.h"
/*
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEFloatBoard
	//----------------------------------------------------------------------------------------------------
	EEFloatBoard::EEFloatBoard(const Rect_Float& _boardRect, int _intRange, int _floatRange, EETexture* _textures)
		:
		EEQuad2D(_boardRect),
		m_intRange(_intRange),
		m_decimalRange(_floatRange),
		m_isRangeDirty(true),
		m_value(0.0f),
		m_isValueDirty(true),
		m_num(_intRange + _floatRange)
	{
		for (int i = 0; i < 11; ++i)
			m_numTexs[i] = _textures[i];
	}

	//----------------------------------------------------------------------------------------------------
	EEFloatBoard::EEFloatBoard(const EEFloatBoard& _floatBoard)
		:
		EEQuad2D(_floatBoard),
		m_intRange(_floatBoard.m_intRange),
		m_decimalRange(_floatBoard.m_decimalRange),
		m_isRangeDirty(_floatBoard.m_isRangeDirty),
		m_value(_floatBoard.m_value),
		m_isValueDirty(_floatBoard.m_isValueDirty),
		m_num(_floatBoard.m_num)
	{
		for (int i = 0; i < 11; ++i)
			m_numTexs[i] = _floatBoard.m_numTexs[i];
	}

	//----------------------------------------------------------------------------------------------------
	EEFloatBoard::~EEFloatBoard()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EEFloatBoard::Update()
	{
		EEQuad2D::Update();

		if (m_isRangeDirty)
		{
			m_num.clear();
			m_num.resize(m_intRange + m_decimalRange + 1);

			int width = (int)(m_quadWidth / m_num.size()); //the size should not be zero
			for (int i = 0; i < m_num.size(); ++i)
			{
				m_num[i].SetParent(this);
				m_num[i].SetLocalZOrder(m_localZOrder); //It is wrong~~
				m_num[i].SetRect(Rect_Float((float)(m_num.size() - i - 1) * width, 0.0f, (float)(m_num.size() - i) * width, (float)m_quadHeight));
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
		//EEQuad2D::Render();

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
*/