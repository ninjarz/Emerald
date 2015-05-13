#pragma once
#ifndef _EE_TEXTEDITER_H_
#define _EE_TEXTEDITER_H_

#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EETextEditer
	//----------------------------------------------------------------------------------------------------
	class EETextEditer : public EEQuad2D
	{
	protected:


	public:
		EETextEditer(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EETextEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor);
		EETextEditer(const EETextEditer& _lineEditer);
		virtual ~EETextEditer();

		virtual bool Update();
		virtual bool Render();

	protected:
		EEFont m_font;
	};
}

#endif