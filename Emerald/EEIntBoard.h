#pragma once
#ifndef _EE_INTBOARD_H_
#define _EE_INTBOARD_H_

#include <vector>
#include "EEQuad2D.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEIntBoard
	//----------------------------------------------------------------------------------------------------
	class EEIntBoard : public EEQuad2D
	{
	public:
		EEIntBoard(const Rect_Float& _boardRect, int _range);
		EEIntBoard(const Rect_Float& _boardRect, int _range, EETexture* _textures);
		EEIntBoard(const EEIntBoard& _intBoard);
		virtual ~EEIntBoard();

		virtual bool Update();
		virtual bool Render();

		bool SetValue(int _value);
		bool SetRange(int _range);
		bool SetNumTextures(EETexture* _textures);

		int GetValue();
		int GetRange();

	protected:
		int m_range;
		bool m_isRangeDirty;
		int m_value;
		bool m_isValueDirty;
		std::vector<EEQuad2D> m_num;
		EETexture m_numTexs[10];
	};
}

#endif