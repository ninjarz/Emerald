#pragma once
#ifndef _EE_ROULETTEWHEEL_H_
#define _EE_ROULETTEWHEEL_H_

#include <vector>


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EERouletteWheel
	//----------------------------------------------------------------------------------------------------
	template <typename _T>
	class EERouletteWheel
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
			float range; // key
			float totalRange;
			NodeColor color;
			_T data;

			//----------------------------------------------------------------------------------------------------
			inline Node(float _range, const _T& _data)
				:
				parent(nullptr),
				left(nullptr),
				right(nullptr),
				range(_range),
				totalRange(_range),
				color(NODE_RED),
				data(_data)
			{}

			//----------------------------------------------------------------------------------------------------
			inline void CalcTotalRange()
			{
				totalRange = _range;
				if (left)
					totalRange += left->totalRange;
				if (right)
					totalRange += right->totalRange;
			}
		};

	public:
		//----------------------------------------------------------------------------------------------------
		inline EERouletteWheel()
			:
			root(nullptr)
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline ~EERouletteWheel()
		{
			DeleteNodes(root);
		}

		//----------------------------------------------------------------------------------------------------
		inline void Insert(float _range, const _T& _data)
		{
			Node *tree = root;
			Node *node = CreateNode(_range, _data);

			while (tree)
			{
				tree->totalRange += _range;
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
			root = node;
			return;
		}

	protected:
		//----------------------------------------------------------------------------------------------------
		inline Node* CreateNode(float _range, const _T& _data)
		{
			return new Node(_range, _data);
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

			if (_node == root)
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
				if (_node == root)
					root = right;
				else if (_node == _node->parent->left)
					_node->parent->left = right;
				else
					_node->parent->right = right;

				// Connect the node to the right node's left
				_node->parent = right;
				right->left = _node;

				// Handle maxValue
				// right->maxValue = _node->maxValue;
				// _node->CalculateMaxValue();
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
				if (_node == root)
					root = left;
				else if (_node == _node->parent->left)
					_node->parent->left = left;
				else
					_node->parent->right = left;

				_node->parent = left;
				left->right = _node;

				//left->maxValue = _node->maxValue;
				//_node->CalculateMaxValue();
			}
		}

	protected:
		Node *root;
	};
};

#endif