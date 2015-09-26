#pragma once
#ifndef _EE_LINEBROWSER_H_
#define _EE_LINEBROWSER_H_

#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EELineBrowser
	//----------------------------------------------------------------------------------------------------
	class EELineBrowser : public EEQuad2D
	{
	public:
		EELineBrowser(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EELineBrowser(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor, const std::string& _text);
		EELineBrowser(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor, const std::wstring& _text);
		EELineBrowser(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor, const std::wstring& _text);
		EELineBrowser(const EELineBrowser& _lineEditor);
		virtual ~EELineBrowser();

		virtual bool Update();
		virtual bool Render();
		bool AddText(wchar_t _text);
		bool AddText(const wchar_t* _text);

		void SetText(const std::string& _text);
		void SetText(const std::wstring& _text);

		const std::wstring& GetText();

	protected:
		EEFont m_font;
	};
}

#endif
