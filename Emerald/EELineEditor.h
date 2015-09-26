#pragma once
#ifndef _EE_LINEEditor_H_
#define _EE_LINEEditor_H_

#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELineEditor
	//----------------------------------------------------------------------------------------------------
	class EELineEditor : public EEQuad2D
	{
	public:
		EELineEditor(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EELineEditor(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor);
		EELineEditor(const EELineEditor& _lineEditor);
		virtual ~EELineEditor();

		virtual bool Update();
		virtual bool Render();

		const std::wstring& GetText();

	protected:
		EEFont m_font;
	};
}

#endif
