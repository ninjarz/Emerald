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
		inline virtual ~EERouletteWheel()
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

		//----------------------------------------------------------------------------------------------------
		inline float GetTotalRange()
		{
			if (m_root)
				return ((Section*)m_root)->totalRange;
			return 0.f;
		}

		//----------------------------------------------------------------------------------------------------
		inline bool Select(float _pos, _T& _result)
		{
			float turns = _pos / GetTotalRange();
			if (turns < 0.f)
				turns += 1.f;
			_pos = GetTotalRange() * (turns - (int)turns);
			return Select(m_root, _pos, _result);
		}

	protected:
		//----------------------------------------------------------------------------------------------------
		inline Section* CreateNode(float _range, const _T& _data)
		{
			return new Section(_range, _data);
		}

		//----------------------------------------------------------------------------------------------------
		inline bool Select(Node* _node, float _pos, _T& _result)
		{
			if (_node)
			{
				// Left
				if (_node->left)
				{
					if (_pos <= ((Section*)_node->left)->totalRange)
						return Select(_node->left, _pos, _result);
					_pos -= ((Section*)_node->left)->totalRange;
				}

				// Self
				if (_pos <= ((Section*)_node)->range)
				{
					_result = _node->data;
					return true;
				}
				_pos -= ((Section*)_node)->range;

				// Right
				if (_node->right)
				{
					if (_pos <= ((Section*)_node->right)->totalRange)
						return Select(_node->right, _pos, _result);
					// Overflow
					return false;
				}
			}

			return false;
		}
	};
};

#endif