#pragma once
#ifndef _EE_TEXTBROWSER_H_
#define _EE_TEXTBROWSER_H_

#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EETextBrowser
	//----------------------------------------------------------------------------------------------------
	class EETextBrowser : public EEQuad2D
	{
	public:
		EETextBrowser(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EETextBrowser(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor);
		EETextBrowser(const EETextBrowser& _textBrowser);
		virtual ~EETextBrowser();

		virtual bool Update();
		virtual bool Render();
		bool AddText(wchar_t _text);
		bool AddText(const wchar_t* _text);

		void SetText(const std::wstring& _text);

		const std::wstring& GetText();

	protected:
		EEFont m_font;
	};
}

#endif