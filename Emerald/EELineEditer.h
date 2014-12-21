#pragma once
#ifndef _EE_LIENEDITER_H_
#define _EE_LIENEDITER_H_

#include "EEGeometry.h"
#include "EEFont.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EELineEditer
	//----------------------------------------------------------------------------------------------------
	class EELineEditer : public EEQuad2D
	{
	protected:


	public:
		EELineEditer(const Rect_Float &_rect, const EEColor& _color, const EEColor& _fontColor);
		EELineEditer(const Rect_Float &_rect, const EETexture& _tex, const EEColor& _fontColor);
		EELineEditer(const EELineEditer& _lineEditer);
		~EELineEditer();

		bool Update();
		bool Render();

	protected:
		EEFont m_text;
	};
}

#endif
