#pragma once
#ifndef _EE_TEXTEditor_H_
#define _EE_TEXTEditor_H_

#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETextEditor
	//----------------------------------------------------------------------------------------------------
	class EETextEditor : public EEQuad2D
	{
	protected:


	public:
		EETextEditor(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EETextEditor(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor);
		EETextEditor(const EETextEditor& _lineEditor);
		virtual ~EETextEditor();

		virtual bool Update();
		virtual bool Render();

	protected:
		EEFont m_font;
	};
}

#endif