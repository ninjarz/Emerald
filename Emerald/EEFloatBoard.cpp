#include "EEFloatBoard.h"

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
		if (!EEQuad2D::Update())
			return false;

		if (m_isRangeDirty)
		{
			m_num.clear();
			m_num.resize(m_intRange + m_decimalRange + 1);

			int width = (int)(m_quadWidth / m_num.size()); //the size should not be zero
			for (unsigned int i = 0; i < m_num.size(); ++i)
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
			float value = m_value >= 0 ? m_value : -m_value;
			int intValue = (int)value;
			for (int i = 0; i < m_intRange; ++i)
			{
				m_num[i].SetTexture(m_numTexs[intValue % 10]);
				intValue /= 10;
			}
			m_num[m_intRange].SetTexture(m_numTexs[10]);
			float decimalValue = value - intValue;
			for (unsigned int i = m_intRange + 1; i < m_num.size(); ++i)
			{
				m_num[i].SetTexture(m_numTexs[intValue % 10]);
				intValue /= 10;
			}

			m_isValueDirty = false;
		}

		for (unsigned int i = 0; i < m_num.size(); ++i)
		{
			m_num[i].Update();
		}


		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFloatBoard::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		for (unsigned int i = 0; i < m_num.size(); ++i)
		{
			m_num[i].Render();
		}

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFloatBoard::SetValue(float _value)
	{
		m_value = _value;
		m_isValueDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFloatBoard::SetIntRange(int _range)
	{
		m_intRange = _range;
		m_isRangeDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFloatBoard::SetDecimalRange(int _range)
	{
		m_decimalRange = _range;
		m_isRangeDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EEFloatBoard::SetRange(int _intRange, int _decimalRange)
	{
		m_intRange = _intRange;
		m_decimalRange = _decimalRange;
		m_isRangeDirty = true;

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	float EEFloatBoard::GetValue()
	{
		return m_value;
	}

	//----------------------------------------------------------------------------------------------------
	int EEFloatBoard::GetIntRange()
	{
		return m_intRange;
	}

	//----------------------------------------------------------------------------------------------------
	int EEFloatBoard::GetDecimalRange()
	{
		return m_decimalRange;
	}
}
