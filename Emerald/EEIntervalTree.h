#pragma once
#ifndef _EE_INTERVALTREE_H_
#define _EE_INTERVALTREE_H_

#include <vector>


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EEIntervalTree
	//----------------------------------------------------------------------------------------------------
	template <typename _T>
	class EEIntervalTree
	{
		//----------------------------------------------------------------------------------------------------
		enum NodeColor
		{
			NODE_RED = 1,
			NODE_BLACK
		};

		//----------------------------------------------------------------------------------------------------
		struct Node
		{
			Node *parent;
			Node *left;
			Node *right;
			float lowValue; // key
			float highValue;
			float maxValue;
			NodeColor color;
			_T data;

			//----------------------------------------------------------------------------------------------------
			inline Node(float _lowValue, float _highValue, const _T& _data)
				: 
				parent(nullptr), 
				left(nullptr), 
				right(nullptr), 
				lowValue(_lowValue),
				highValue(_highValue),
				maxValue(_highValue),
				color(NODE_RED), 
				data(_data)
			{}

			//----------------------------------------------------------------------------------------------------
			inline void CalcMaxValue()
			{
				maxValue = highValue;
				if (left)
					maxValue = max(maxValue, left->maxValue);
				if (right)
					maxValue = max(maxValue, right->maxValue);
			}
		};

	public:
		//----------------------------------------------------------------------------------------------------
		inline EEIntervalTree()
			:
			m_root(nullptr)
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline ~EEIntervalTree()
		{
			DeleteNodes(m_root);
		}

		//----------------------------------------------------------------------------------------------------
		inline void Insert(float _lowValue, float _highValue, const _T& _data)
		{
			Node *tree = m_root;
			Node *node = CreateNode(_lowValue, _highValue, _data);

			while (tree)
			{
				tree->maxValue = max(tree->maxValue, _highValue);
				if (_lowValue < tree->lowValue)
				{
					if (tree->left)
						tree = tree->left;
					else
					{
						node->parent = tree;
						tree->left = node;
						FixInsert(node);
						return;
					}
				}
				else
				{
					if (tree->right)
						tree = tree->right;
					else
					{
						node->parent = tree;
						tree->right = node;
						FixInsert(node);
						return;
					}
				}
			}

			node->color = NODE_BLACK;
			m_root = node;
			return;
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
			return new Node(_lowValue, _highValue, _data);
		}

		//----------------------------------------------------------------------------------------------------
		inline void DeleteNode(Node* _node)
		{
			if (!_node->left || !_node->right)
			{

			}
		}

		//----------------------------------------------------------------------------------------------------
		inline void DeleteNodes(Node* _node)
		{
			if (_node)
			{
				if (_node->parent)
				{
					if (_node == _node->parent->left)
						_node->parent->left = nullptr;
					else
						_node->parent->right = nullptr;
				}

				DeleteNodes(_node->left);
				DeleteNodes(_node->right);
				delete _node;
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline void FixInsert(Node* _node)
		{
			while (_node->parent && _node->parent->color == NODE_RED)
			{
				if (_node->parent == _node->parent->parent->left)
				{
					// 1
					Node *uncle = _node->parent->parent->right;
					if (uncle && uncle->color == NODE_RED)
					{
						uncle->color = NODE_BLACK;
						_node->parent->color = NODE_BLACK;
						_node->parent->parent->color = NODE_RED;
						_node = _node->parent->parent;
					}
					else
					{
						// 2
						if (_node == _node->parent->right)
						{
							_node = _node->parent;
							LeftRotate(_node);
						}
						// 3
						else
						{
							_node->parent->color = NODE_BLACK;
							_node->parent->parent->color = NODE_RED;
							RightRotate(_node->parent->parent);
							return;
						}
					}
				}
				else
				{
					// 1
					Node *uncle = _node->parent->parent->left;
					if (uncle && uncle->color == NODE_RED)
					{
						uncle->color = NODE_BLACK;
						_node->parent->color = NODE_BLACK;
						_node->parent->parent->color = NODE_RED;
						_node = _node->parent->parent;
					}
					else
					{
						// 2
						if (_node == _node->parent->left)
						{
							_node = _node->parent;
							RightRotate(_node);
						}
						// 3
						else
						{
							_node->parent->color = NODE_BLACK;
							_node->parent->parent->color = NODE_RED;
							LeftRotate(_node->parent->parent);
							return;
						}
					}
				}
			}

			if (_node == m_root)
			{
				_node->color = NODE_BLACK;
				return;
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline void LeftRotate(Node* _node)
		{
			if (_node->right)
			{
				Node *right = _node->right;

				// Connect the right node's left node to the node's right
				if (right->left)
					right->left->parent = _node;
				_node->right = right->left;

				// Connect the right node to the child of node's parent
				right->parent = _node->parent;
				if (_node == m_root)
					m_root = right;
				else if (_node == _node->parent->left)
					_node->parent->left = right;
				else
					_node->parent->right = right;

				// Connect the node to the right node's left
				_node->parent = right;
				right->left = _node;

				// Handle maxValue
				right->maxValue = _node->maxValue;
				_node->CalcMaxValue();
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline void RightRotate(Node* _node)
		{
			if (_node->left)
			{
				Node *left = _node->left;

				if (left->right)
					left->right->parent = _node;
				_node->left = left->right;

				left->parent = _node->parent;
				if (_node == m_root)
					m_root = left;
				else if (_node == _node->parent->left)
					_node->parent->left = left;
				else
					_node->parent->right = left;

				_node->parent = left;
				left->right = _node;

				left->maxValue = _node->maxValue;
				_node->CalcMaxValue();
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline bool FindData(Node* _node, float _lowValue, float _highValue, _T& _data)
		{
			if (_node && _lowValue <= _node->maxValue)
			{
				if (Overlap(*_node, _lowValue, _highValue))
				{
					_data = _node->data;
					return true;
				}

				if (FindData(_node->left, _lowValue, _highValue, _data))
				{
					return true;
				}
				if (_node->lowValue <= _highValue)
				{
					return FindData(_node->right, _lowValue, _highValue, _data);
				}
			}

			return false;
		}

		//----------------------------------------------------------------------------------------------------
		inline void FindData(Node* _node, float _lowValue, float _highValue, std::vector<_T>& _data)
		{
			if (_node && _lowValue <= _node->maxValue)
			{
				if (Overlap(*_node, _lowValue, _highValue))
				{
					_data.push_back(_node->data);
				}

				FindData(_node->left, _lowValue, _highValue, _data);
				if (_node->lowValue <= _highValue)
				{
					FindData(_node->right, _lowValue, _highValue, _data);
				}
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline bool Overlap(const Node& _nodeA, const Node& _nodeB)
		{
			if (_nodeA.highValue < _nodeB.lowValue)
				return false;
			else if (_nodeB.highValue < _nodeA.lowValue)
				return false;

			return true;
		}

		//----------------------------------------------------------------------------------------------------
		inline bool Overlap(const Node& _node, float _lowValue, float _highValue)
		{
			if (_node.highValue < _lowValue)
				return false;
			else if (_highValue < _node.lowValue)
				return false;

			return true;
		}

	protected:
		Node *m_root;
	};
};

#endif