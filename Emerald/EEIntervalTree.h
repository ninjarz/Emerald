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
			NODE_RED,
			NODE_BLACK
		};

		//----------------------------------------------------------------------------------------------------
		struct Node
		{
			float leftValue;
			Node *leftChild;
			float rightValue;
			Node *rightChild;
			float maxValue;
			NodeColor color;
			_T data;

			inline Node() : leftValue(0.f), leftChild(nullptr), rightValue(0.f), rightChild(nullptr), maxValue(0.f), color(NODE_RED), data() {}
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

		inline bool Insert(int _leftValue, int _rightValue, const _T& _data)
		{
			Node *tree = root;

			while (tree)
			{

			}

			return false;
		}
		
	protected:
		Node* CreateNode(int _left, int _right, const _T& _value)
		{
			return nullptr;
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