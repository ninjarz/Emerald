#pragma once
#ifndef _EE_REDBLACKTREE_H_
#define _EE_REDBLACKTREE_H_

#include <vector>


//----------------------------------------------------------------------------------------------------
namespace Emerald
{
	// EERedBlackTree
	//----------------------------------------------------------------------------------------------------
	template <typename _T>
	class EERedBlackTree
	{
	protected:
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
			NodeColor color;
			_T data; // key

			//----------------------------------------------------------------------------------------------------
			inline Node(const _T& _data)
				:
				parent(nullptr),
				left(nullptr),
				right(nullptr),
				color(NODE_RED),
				data(_data)
			{}
		};

	public:
		//----------------------------------------------------------------------------------------------------
		inline EERedBlackTree()
			:
			m_root(nullptr)
		{
		}

		//----------------------------------------------------------------------------------------------------
		inline ~EERedBlackTree()
		{
			DeleteNodes(m_root);
		}

		//----------------------------------------------------------------------------------------------------
		inline void Insert(const _T& _data)
		{
			Node *tree = m_root;
			Node *node = CreateNode(_data);

			while (tree)
			{
				// tree->maxValue = max(tree->maxValue, _highValue);
				if (node->data < tree->data)
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
		inline bool Delete(const _T& _data)
		{
			Node *target = FindNode(m_root, _data);
			if (target)
			{
				DeleteNode(target);
				return true;
			}

			return false;
		}

		//----------------------------------------------------------------------------------------------------
		inline void GetData(std::vector<_T>& _data)
		{
			_data.clear();
			GetData(m_root, _data);
		}

	protected:
		//----------------------------------------------------------------------------------------------------
		inline Node* CreateNode(const _T& _data)
		{
			return new Node(_data);
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
				if (_node == m_root)
					m_root = left;
				else if (_node == _node->parent->left)
					_node->parent->left = left;
				else
					_node->parent->right = left;

				_node->parent = left;
				left->right = _node;

				// left->maxValue = _node->maxValue;
				// _node->CalculateMaxValue();
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline void DeleteNode(Node* _node)
		{
			if (_node)
			{
				if (_node->left && _node->right)
				{
					Node *successor = FindSuccessor(_node->right);
					_node->data = successor->data;
					DeleteNode(successor);
				}
				else
				{
					Node *child = _node->left ? _node->left : _node->right;
					if (child)
					{
						child->parent = _node->parent;
						if (_node == m_root)
							m_root = child;
						else if (_node == _node->parent->left)
							_node->parent->left = child;
						else
							_node->parent->right = child;

						if (_node->color == NODE_BLACK) // Black
						{
							if (child->color == NODE_RED)
							{
								child->color = NODE_BLACK;
							}
							else // Double black (child -1 black)
							{
								while (child != m_root)
								{
									Node *sibling = FindSibling(child);
									if (sibling->color == NODE_RED)
									{
										sibling->color = NODE_BLACK;
										child->parent->color = NODE_RED;
										if (child == child->parent->left)
											LeftRotate(child->parent);
										else
											RightRotate(child->parent);
									}
									else
									{
										if ((!sibling->left || sibling->left->color == NODE_BLACK) && (!sibling->right || sibling->right->color == NODE_BLACK))
										{
											if (child->parent->color == NODE_BLACK)  // Keep balance between child and sibling (child's parent -1 black)
											{
												sibling->color = NODE_RED;
												child = child->parent;
												continue;
											}
											else
											{
												sibling->color = NODE_RED;
												child->parent->color = NODE_BLACK;
												break;
											}
										}
										else
										{
											if (child == child->parent->left)
											{
												if ((sibling->left && sibling->left->color == NODE_RED) && (!sibling->right || sibling->right->color == NODE_BLACK))
												{
													sibling->color = NODE_RED;
													sibling->left->color = NODE_BLACK;
													RightRotate(sibling);
												}

												sibling = FindSibling(child);
												sibling->color = sibling->parent->color;
												child->parent->color = NODE_BLACK;
												sibling->right->color = NODE_BLACK;
												LeftRotate(child->parent);
												break;
											}
											else
											{
												if ((!sibling->left && sibling->left->color == NODE_BLACK) && (sibling->right || sibling->right->color == NODE_RED))
												{
													sibling->color = NODE_RED;
													sibling->right->color = NODE_BLACK;
													LeftRotate(sibling);
												}

												sibling = FindSibling(child);
												sibling->color = sibling->parent->color;
												child->parent->color = NODE_BLACK;
												sibling->left->color = NODE_BLACK;
												RightRotate(child->parent);
												break;
											}
										}
									}
								}
							}
						}
					}

					if (_node == m_root)
						m_root = nullptr;
					delete _node;
				}
			}
		}

		//----------------------------------------------------------------------------------------------------
		inline void DeleteNodes(Node* _node) // Todo: use DeleteNode
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
		inline Node* FindPredecessor(Node* _node)
		{
			if (_node)
			{
				if (_node->right == nullptr) // parent X
					return _node;
				return FindPredecessor(_node->right);
			}

			return nullptr;
		}

		//----------------------------------------------------------------------------------------------------
		inline Node* FindSuccessor(Node* _node)
		{
			if (_node)
			{
				if (_node->left == nullptr) // parent X
					return _node;
				return FindSuccessor(_node->left);
			}

			return nullptr;
		}

		//----------------------------------------------------------------------------------------------------
		inline Node* FindSibling(Node* _node)
		{
			if (_node && _node->parent)
			{
				if (_node == _node->parent->left)
					return _node->parent->right;
				else
					return _node->parent->left;
			}

			return nullptr;
		}

		//----------------------------------------------------------------------------------------------------
		inline Node* FindNode(Node* _node, const _T& _data)
		{
			if (_node)
			{
				if (_node->data == _data)
				{
					return _node;
				}
				else if (_data < _node->data)
				{
					return FindNode(_node->left, _data);
				}
				else
				{
					return FindNode(_node->right, _data);
				}
			}

			return nullptr;
		}

		//----------------------------------------------------------------------------------------------------
		inline void GetData(Node *node, std::vector<_T>& _data)
		{
			if (node)
			{
				GetData(node->left, _data);
				_data.push_back(node->data);
				GetData(node->right, _data);
			}
		}

	protected:
		Node *m_root;
	};
};

#endif