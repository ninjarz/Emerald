#pragma once
#ifndef _EE_LINEEDITER_H_
#define _EE_LINEEDITER_H_

#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELineEditer
	//----------------------------------------------------------------------------------------------------
	class EELineEditer : public EEQuad2D
	{
	public:
		EELineEditer(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EELineEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor);
		EELineEditer(const EELineEditer& _lineEditer);
		virtual ~EELineEditer();

		virtual bool Update();
		virtual bool Render();

		const std::wstring& GetText();

	protected:
		EEFont m_font;
	};
}

#endif
