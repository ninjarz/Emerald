#include "EETextBrowser.h"
#include "EECore.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EETextBrowser
	//----------------------------------------------------------------------------------------------------
	EETextBrowser::EETextBrowser(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _color),
		m_font(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EETextBrowser::EETextBrowser(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor)
		:
		EEQuad2D(_rect, _tex),
		m_font(FLOAT3(0.0f, 0.0f, 0.0f), _fontColor, L"")
	{
		m_font.SetParent(this);
	}

	//----------------------------------------------------------------------------------------------------
	EETextBrowser::EETextBrowser(const EETextBrowser& _textBrowser)
		:
		EEQuad2D(_textBrowser),
		m_font(_textBrowser.m_font)
	{

	}

	//----------------------------------------------------------------------------------------------------
	EETextBrowser::~EETextBrowser()
	{

	}

	//----------------------------------------------------------------------------------------------------
	bool EETextBrowser::Update()
	{
		if (!EEQuad2D::Update())
			return false;

		if (m_isTriggered)
		{
			//the key queue need to be clear here
			m_isTriggered = false;
		}
		if (s_focusedObject == this)
		{

		}
		m_font.Update();

		return true;
	}

	//----------------------------------------------------------------------------------------------------
	bool EETextBrowser::Render()
	{
		if (!EEQuad2D::Render())
			return false;

		return m_font.Render();
	}

	//----------------------------------------------------------------------------------------------------
	bool EETextBrowser::AddText(wchar_t _text)
	{
		return m_font.AddText(_text);
	}

	//----------------------------------------------------------------------------------------------------
	bool EETextBrowser::AddText(const wchar_t* _text)
	{
		return m_font.AddText(_text);
	}

	//----------------------------------------------------------------------------------------------------
	void EETextBrowser::SetText(const std::wstring& _text)
	{
		m_font.SetText(_text.data());
	}

	//----------------------------------------------------------------------------------------------------
	const std::wstring& EETextBrowser::GetText()
	{
		return m_font.GetText();
	}
}