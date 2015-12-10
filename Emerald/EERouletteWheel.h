#pragma once
#ifndef _EE_ROULETTEWHEEL_H_
#define _EE_ROULETTEWHEEL_H_

#include <vector>
#include <iterator>
#include "EERedBlackTree.h"


// Todo: size
//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EERouletteWheel
	//----------------------------------------------------------------------------------------------------
	template <typename _T>
	class EERouletteWheel : protected EERedBlackTree<_T>
	{
	protected:
		//----------------------------------------------------------------------------------------------------
		struct Section : public Node
		{
			// key: range
			float totalRange;

			//----------------------------------------------------------------------------------------------------
			inline Section(float _range, const _T& _data)
				:
				Node(_range, _data),
				totalRange(_range)
			{
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual bool AssignData(Node* _node)
			{
				if (Node::AssignData(_node))
				{
					Calculate();
				}
				return true;
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual void Calculate()
			{
				totalRange = key;
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
		inline NodeIterator begin()
		{
			return EERedBlackTree<_T>::begin();
		}

		//----------------------------------------------------------------------------------------------------
		inline NodeIterator end()
		{
			return EERedBlackTree<_T>::end();
		}

		//----------------------------------------------------------------------------------------------------
		inline _T& Front()
		{
			return EERedBlackTree<_T>::Front();
		}

		//----------------------------------------------------------------------------------------------------
		inline _T& Back()
		{
			return EERedBlackTree<_T>::Back();
		}

		//----------------------------------------------------------------------------------------------------
		inline void PopFront()
		{
			return EERedBlackTree<_T>::PopFront();
		}

		//----------------------------------------------------------------------------------------------------
		inline unsigned int Size()
		{
			return EERedBlackTree<_T>::Size();
		}

		//----------------------------------------------------------------------------------------------------
		inline void Insert(float _range, const _T& _data)
		{
			EERedBlackTree<_T>::Insert(CreateNode(_range, _data));
		}

		//----------------------------------------------------------------------------------------------------
		inline bool Delete(float _range, const _T& _data)
		{
			Node *target = EERedBlackTree<_T>::FindNode(m_root, &Section(_range, _data));
			if (target)
			{
				EERedBlackTree<_T>::Delete(target);
				return true;
			}
			return false;
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

		//----------------------------------------------------------------------------------------------------
		inline bool RandomSelect(_T& _result)
		{
			float pos = GetTotalRange() * ((float)rand() / (float)RAND_MAX);
			return Select(pos, _result);
		}

	protected:
		//----------------------------------------------------------------------------------------------------
		inline virtual Node* CreateNode(float _range, const _T& _data)
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
				if (_pos <= ((Section*)_node)->key)
				{
					_result = _node->data;
					return true;
				}
				_pos -= ((Section*)_node)->key;

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