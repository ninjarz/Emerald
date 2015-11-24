#pragma once
#ifndef _EE_HASH_H_
#define _EE_HASH_H_


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
			float leftValue; // key
			float rightValue;
			float maxValue;
			NodeColor color;
			_T data;

			inline Node(float _leftValue, float _rightValue, const _T& _data)
				: 
				parent(nullptr), 
				left(nullptr), 
				right(nullptr), 
				leftValue(_leftValue),
				rightValue(_rightValue),
				maxValue(_rightValue),
				color(NODE_RED), 
				data(_data)
			{}
		};

	public:
		inline EEIntervalTree()
			:
			root(nullptr)
		{
		}

		inline ~EEIntervalTree()
		{
		}

		inline void Insert(float _leftValue, float _rightValue, const _T& _data)
		{
			Node *tree = root;
			Node *node = CreateNode(_leftValue, _rightValue, _data);

			while (tree)
			{
				tree->maxValue = max(tree->maxValue, _rightValue);
				if (_leftValue < tree->leftValue)
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
		Node* CreateNode(float _leftValue, float _rightValue, const _T& _data)
		{
			return new Node(_leftValue, _rightValue, _data);
		}

		void FixInsert(Node* _node)
		{
			while (_node->parent->color == NODE_RED)
			{

			}
		}

		void LeftRotate(Node* _node)
		{
			Node *tmp = _node->right;

		}

		void RightRotate(Node* _node)
		{

		}

		bool Overlap(const Node& _a, const Node& _b)
		{
			if (_a.rightValue < _b.leftValue)
				return false;
			else if (_b.rightValue < _a.leftValue)
				return false;

			return true;
		}

	protected:
		Node *root;
	};
}

#endif