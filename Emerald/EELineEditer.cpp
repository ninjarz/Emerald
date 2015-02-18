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
		m_text(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, "")
	{
		m_text.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EELineEditer::EELineEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _tex),
		m_text(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, "")
	{
		m_text.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EELineEditer::EELineEditer(const EELineEditer& _lineEditer)
		:
		EEQuad2D(_lineEditer),
		m_text(_lineEditer.m_text)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EELineEditer::~EELineEditer()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EELineEditer::Update()
	{
		EEQuad2D::Update();

		if (m_isTriggered)
		{
			//the key queue need to be clear here
			m_isTriggered = false;
		}
		if (s_focusedObject == this)
		{
			if (EECore::s_EECore->IsKeyInput())
			{
				//the letter on the LineEditer can not be '\r'
				char letter = (char)EECore::s_EECore->GetKey();
				if (letter != '\r' && letter != '\n')
				{
					m_text.AddText(letter);
				}
			}
		}
		m_text.Update();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EELineEditer::Render()
	{
		EEQuad2D::Render();
		m_text.Render();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	const std::string& EELineEditer::GetText()
	{
		return m_text.GetText();
	}
}