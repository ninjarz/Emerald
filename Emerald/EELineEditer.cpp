#include "EELineEditer.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELineEditer
	//----------------------------------------------------------------------------------------------------
	EELineEditer::EELineEditer(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _color),
		m_font(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EELineEditer::EELineEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _tex),
		m_font(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EELineEditer::EELineEditer(const EELineEditer& _lineEditer)
		:
		EEQuad2D(_lineEditer),
		m_font(_lineEditer.m_font)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EELineEditer::~EELineEditer()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EELineEditer::Update()
	{
		if (!EEQuad2D::Update())
			return false;

		if (s_focusedObject == this)
		{
			//the key queue need to be clear here
		}
		if (s_triggeredObject == this)
		{
			while (EECore::s_EECore->IsKeyInput())
			{
				//the letter on the LineEditer can not be '\r'
				wchar_t letter = (wchar_t)EECore::s_EECore->GetKey();
				if (letter != L'\r' && letter != L'\n')
				{
					m_font.AddText(letter);
				}
			}
		}
		m_font.Update();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EELineEditer::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		return m_font.Render();
	}

	//----------------------------------------------------------------------------------------------------
	const std::wstring& EELineEditer::GetText()
	{
		return m_font.GetText();
	}
}