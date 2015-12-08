#pragma once
#ifndef _EE_INTERVALTREE_H_
#define _EE_INTERVALTREE_H_

#include <vector>
#include "EERedBlackTree.h"


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEIntervalTree
	//----------------------------------------------------------------------------------------------------
	template <typename _T>
	class EEIntervalTree : protected EERedBlackTree<_T>
	{
	protected:
		//----------------------------------------------------------------------------------------------------
		struct Interval : public Node
		{
			// key: lowValue
			float highValue;
			float maxValue;

			//----------------------------------------------------------------------------------------------------
			inline Interval(float _lowValue, float _highValue, const _T& _data)
				: 
				Node(_lowValue, _data),
				highValue(_highValue),
				maxValue(_highValue)
			{}

			//----------------------------------------------------------------------------------------------------
			inline virtual bool AssignData(Node* _node)
			{
				if (Node::AssignData(_node))
				{
					highValue = ((Interval*)_node)->highValue;
					Calculate();
				}
				return true;
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual void Calculate()
			{
				maxValue = highValue;
				if (left)
					maxValue = max(maxValue, ((Interval*)left)->maxValue);
				if (right)
					maxValue = max(maxValue, ((Interval*)right)->maxValue);
			}

			//----------------------------------------------------------------------------------------------------
			inline virtual void HandleInsert(Node* _node)
			{
				if (_node)
				{
					maxValue = max(maxValue, ((Interval*)_node)->highValue);
				}
			}
		};

	public:
		//----------------------------------------------------------------------------------------------------
		inline EEIntervalTree()
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline virtual ~EEIntervalTree()
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline void Insert(float _lowValue, float _highValue, const _T& _data)
		{
			EERedBlackTree<_T>::Insert(CreateNode(_lowValue, _highValue, _data));
		}

		//----------------------------------------------------------------------------------------------------
		inline void FindData(float _lowValue, float _highValue, std::vector<_T>& _data)
		{
			_data.clear();
			FindData(m_root, _lowValue, _highValue, _data);
		}

		//----------------------------------------------------------------------------------------------------
		inline bool FindData(float _lowValue, float _highValue, _T& _data)
		{
			return FindData(m_root, _lowValue, _highValue, _data);
		}
		
	protected:
		//----------------------------------------------------------------------------------------------------
		inline Node* CreateNode(float _lowValue, float _highValue, const _T& _data)
		{
			return new Interval(_lowValue, _highValue, _data);
		}

		//----------------------------------------------------------------------------------------------------
		inline bool FindData(Node* _node, float _lowValue, float _highValue, _T& _data)
		{
			if (_node && _lowValue <= ((Interval*)_node)->maxValue)
			{
				if (Overlap(_node, _lowValue, _highValue))
				{
					_data = _node->data;
					return true;
				}

				if (FindData(_node->left, _lowValue, _highValue, _data))
				{
					return true;
				}
				if (((Interval*)_node)->key <= _highValue)
				{
					return FindData(_node->right, _lowValue, _highValue, _data);
				}
			}

			return false;
		}

		//----------------------------------------------------------------------------------------------------
		inline void FindData(Node* _node, float _lowValue, float _highValue, std::vector<_T>& _data)
		{
			if (_node && _lowValue <= ((Interval*)_node)->maxValue)
			{
				if (Overlap(_node, _lowValue, _highValue))
				{
					_data.push_back(_node->data);
				}

				FindData(_node->left, _lowValue, _highValue, _data);
				if (((Interval*)_node)->key <= _highValue)
				{
					FindData(_node->right, _lowValue, _highValue, _data);
				}
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline bool Overlap(Node* _node, float _lowValue, float _highValue)
		{
			if (((Interval*)_node)->highValue < _lowValue)
				return false;
			else if (_highValue < ((Interval*)_node)->key)
				return false;

			return true;
		}
	};
};

#endif