#include "EELineBrowser.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELineBrowser
	//----------------------------------------------------------------------------------------------------
	EELineBrowser::EELineBrowser(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _color),
		m_font(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EELineBrowser::EELineBrowser(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor, const std::wstring& _text)
		:
		EEQuad2D(_rect, _color),
		m_font(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
		m_font.SetText(_text);
	}

	//----------------------------------------------------------------------------------------------------
	EELineBrowser::EELineBrowser(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor, const std::wstring& _text)
		:
		EEQuad2D(_rect, _tex),
		m_font(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
		m_font.SetText(_text);
	}

	//----------------------------------------------------------------------------------------------------
	EELineBrowser::EELineBrowser(const EELineBrowser& _lineEditer)
		:
		EEQuad2D(_lineEditer),
		m_font(_lineEditer.m_font)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EELineBrowser::~EELineBrowser()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EELineBrowser::Update()
	{
		if (!EEQuad2D::Update())
			return false;

		if (s_focusedObject == this)
		{
			//the key queue need to be clear here
		}
		if (s_triggeredObject == this)
		{

		}
		m_font.Update();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EELineBrowser::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		return m_font.Render();
	}

	//----------------------------------------------------------------------------------------------------
	bool EELineBrowser::AddText(wchar_t _text)
	{
		return m_font.AddText(_text);
	}

	//----------------------------------------------------------------------------------------------------
	bool EELineBrowser::AddText(const wchar_t* _text)
	{
		return m_font.AddText(_text);
	}

	//----------------------------------------------------------------------------------------------------
	void EELineBrowser::SetText(const std::wstring& _text)
	{
		m_font.SetText(_text.data());
	}

	//----------------------------------------------------------------------------------------------------
	const std::wstring& EELineBrowser::GetText()
	{
		return m_font.GetText();
	}
}