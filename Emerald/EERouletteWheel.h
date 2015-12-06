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
			inline virtual bool AssignData(Node* _node)
			{
				if (Node::AssignData(_node))
				{
					range = ((Section*)_node)->range;
					Calculate();
				}
				return true;
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual bool Equal(Node* _node) const
			{
				if (_node)
				{
					return range == ((Section*)_node)->range;
				}
				return false;
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual bool Less(Node* _node) const
			{
				if (_node)
				{
					return range < ((Section*)_node)->range;
				}
				return false;
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual void Calculate()
			{
				totalRange = range;
				if (left)
					totalRange += ((Section*)left)->totalRange;
				if (right)
					totalRange += ((Section*)right)->totalRange;

				if (parent)
					parent->Calculate();
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual void HandleInsert(Node* _node)
			{
				if (_node)
				{
					totalRange += ((Section*)_node)->totalRange;
				}
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

		//----------------------------------------------------------------------------------------------------
		inline bool Delete(const _T& _data)
		{
			return EERedBlackTree<_T>::Delete(_data);
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