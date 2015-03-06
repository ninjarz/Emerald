#ifndef _EE_FLOATBOARD_H_
#define _EE_FLOATBOARD_H_

#include <vector>
#include "EEQuad2D.h"

//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	//EEFloatBoard
	//----------------------------------------------------------------------------------------------------
	class EEFloatBoard : public EEQuad2D
	{
	public:
		EEFloatBoard(const Rect_Float& _boardRect, int _intRange, int _decimalRange, EETexture* _textures);
		EEFloatBoard(const EEFloatBoard& _floatBoard);
		~EEFloatBoard();

		bool Update();
		bool Render();

		bool SetValue(float _value);
		bool SetIntRange(int _range);
		bool SetDecimalRange(int _range);
		bool SetRange(int _intRange, int _decimalRange);

		float GetValue();
		int GetIntRange();
		int GetDecimalRange();

	protected:
		int m_intRange;
		int m_decimalRange;
		bool m_isRangeDirty;
		float m_value;
		bool m_isValueDirty;
		std::vector<EEQuad2D> m_num;
		EETexture m_numTexs[11];
	};
}


#endif