#include "EELineEditor.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELineEditor
	//----------------------------------------------------------------------------------------------------
	EELineEditor::EELineEditor(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _color),
		m_font(m_quadWidth, m_quadHeight, _fontColor, L"")
	{
		m_font.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EELineEditor::EELineEditor(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _tex),
		m_font(m_quadWidth, m_quadHeight, _fontColor, L"")
	{
		m_font.SetParent(this);
		SetIsFocusable(true);
	}

	//----------------------------------------------------------------------------------------------------
	EELineEditor::EELineEditor(const EELineEditor& _lineEditor)
		:
		EEQuad2D(_lineEditor),
		m_font(_lineEditor.m_font)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EELineEditor::~EELineEditor()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EELineEditor::Update()
	{
		if (!EEQuad2D::Update())
			return false;

		if (s_focusedObject == this)
		{
			// the key queue need to be cleared here
		}
		if (s_triggeredObject == this)
		{
			while (EECore::s_EECore->IsKeyInput())
			{
				//the letter on the LineEditor can not be '\r'
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
	bool EELineEditor::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		return m_font.Render();
	}

	//----------------------------------------------------------------------------------------------------
	const std::wstring& EELineEditor::GetText()
	{
		return m_font.GetText();
	}
}