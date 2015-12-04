#pragma once
#ifndef _EE_ROULETTEWHEEL_H_
#define _EE_ROULETTEWHEEL_H_

#include <vector>
#include "EERedBlackTree.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EERouletteWheel
	//----------------------------------------------------------------------------------------------------
	template <typename _T>
	class EERouletteWheel : protected EERedBlackTree<_T>
	{
		//----------------------------------------------------------------------------------------------------
		struct Section : public Node
		{
			float range; // key
			float totalRange;

			//----------------------------------------------------------------------------------------------------
			inline Section(float _range, const _T& _data)
				:
				Node(_data),
				range(_range),
				totalRange(_range)
			{}

			//----------------------------------------------------------------------------------------------------
			inline virtual void HandleInsert()
			{
				int i = 0;
			}
		};

	public:
		//----------------------------------------------------------------------------------------------------
		inline EERouletteWheel()
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline ~EERouletteWheel()
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline void Insert(float _range, const _T& _data)
		{
			Section *node = CreateNode(_range, _data);
			EERedBlackTree<_T>::Insert(node);
		}


	protected:
		//----------------------------------------------------------------------------------------------------
		inline Section* CreateNode(float _range, const _T& _data)
		{
			return new Section(_range, _data);
		}
	};
};

#endif