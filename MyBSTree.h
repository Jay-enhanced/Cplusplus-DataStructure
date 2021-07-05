#pragma once
#include "MyException.h"
#include "MyStack.h"
#include <iostream>

using namespace std;

// 节点
template <class _Ty>
class MyBSTreeNode
{
public:
	MyBSTreeNode(_Ty key, MyBSTreeNode* parent, MyBSTreeNode* lchild, MyBSTreeNode* rchild)
	{
		this->key = key;
		this->parent = parent;
		this->lchild = lchild;
		this->rchild = rchild;
	}

	_Ty key;
	MyBSTreeNode* parent;
	MyBSTreeNode* lchild;
	MyBSTreeNode* rchild;
};

// 二叉树
template <class _Ty>
class MyBSTree
{
public:
	MyBSTree();
	~MyBSTree();

	// 1.插入键值为key的节点
	void insert(_Ty key);
	// 2.删除键值为key的节点
	void remove(_Ty key);
	// 3.先根遍历
	void preorder();			// 递归版本
	void preorder_iterative();	// 迭代版本
	// 4.中根遍历
	void inorder();				// 递归版本
	void inorder_iterative();	// 迭代版本
	// 5.后根遍历
	void postorder();			// 递归版本
	void postorder_iterative();	// 迭代版本
	// 6.返回二叉树中的最小值
	_Ty min() const;
	// 7.返回二叉树中的最大值
	_Ty max() const;
	// 8.查找键值key是否在二叉树中
	bool contains(_Ty key) const;
	// 9.构建二叉树
	bool create(_Ty root_key, _Ty keys[], unsigned int size);
	// 10.销毁二叉树
	void destroy();
	// 11.返回树的大小
	int size();

private:
	// 私有成员和方法，类内部使用
	MyBSTreeNode<_Ty>* root;
	int m_size;
	// 1.查找键值为key的节点
	MyBSTreeNode<_Ty>* search(_Ty key) const;
	// 2.先根遍历
	void preorder(MyBSTreeNode<_Ty>* node);
	// 3.中根遍历
	void inorder(MyBSTreeNode<_Ty>* node);
	// 4.后根遍历
	void postorder(MyBSTreeNode<_Ty>* node);
	// 5.销毁二叉树
	void destroy(MyBSTreeNode<_Ty>* node);
};

// 构造函数
template <class _Ty>
MyBSTree<_Ty>::MyBSTree()
{
	m_size = 0;
	root = nullptr;
}

// 析构函数
template <class _Ty>
MyBSTree<_Ty>::~MyBSTree()
{
	destroy();
}

// 1.插入键值为key的节点
template<class _Ty>
void MyBSTree<_Ty>::insert(_Ty key)
{
	MyBSTreeNode<_Ty>* _new = new MyBSTreeNode<_Ty>(key, nullptr, nullptr, nullptr);
	if (!_new)
	{
		throw MyException("Failed to create new node.");
	}

	MyBSTreeNode<_Ty>* _root = root;
	MyBSTreeNode<_Ty>* _tmp = nullptr;

	while (_root)
	{
		_tmp = _root;
		if (key == _root->key)
		{
			delete _new;
			_new = nullptr;
			return;
		}

		if (key < _root->key)
		{
			_root = _root->lchild;
		}
		else
		{
			_root = _root->rchild;
		}
	}

	_new->parent = _tmp;
	if (_tmp == nullptr)
	{
		root = _new;
	}
	else if (key < _tmp->key)
	{
		_tmp->lchild = _new;
	}
	else
	{
		_tmp->rchild = _new;
	}
	m_size++;
}

// 2.删除键值为key的节点
template<class _Ty>
void MyBSTree<_Ty>::remove(_Ty key)
{
	MyBSTreeNode<_Ty>* _node = search(key);
	if (_node == nullptr)
	{
		throw MyException("The node is not found and can not be deleted.");
	}

	// 被删除的节点是叶子节点，直接删除
	if (_node->lchild == nullptr && _node->rchild == nullptr)
	{
		if (_node == root)
		{
			delete root;
			root = nullptr;
			m_size = 0;
			return;
		}

		if (_node->parent->lchild == _node)
		{
			_node->parent->lchild = nullptr;
		}
		else
		{
			_node->parent->rchild = nullptr;
		}

		delete _node;
		_node = nullptr;
		m_size--;
		return;
	}

	// 被删除的节点是非叶子节点，需要从左右子树中找到键值仅次于key的节点作为晋升（喜当爹）节点
	MyBSTreeNode<_Ty>* _promotion_node = nullptr;
	if (_node->rchild != nullptr)
	{
		// 右子树非空，从右子树找的左半边（包括子树的根）找晋升（喜当爹）节点
		MyBSTreeNode<_Ty>* _tmp = _node->rchild;
		while (1)
		{
			if (_tmp->lchild != nullptr)
			{
				_tmp = _tmp->lchild;
			}
			else
			{
				_promotion_node = _tmp;
				break;
			}
		}
	}
	else
	{
		// 右子树是空的，需要从左子树的右半边（包括子树的根）找晋升节点
		MyBSTreeNode<_Ty>* _tmp = _node->lchild;
		while (1)
		{
			if (_tmp->rchild != nullptr)
			{
				_tmp = _tmp->rchild;
			}
			else
			{
				_promotion_node = _tmp;
				break;
			}
		}
	}

	// 晋升节点是从右子树找到的，有两种情况，一是叶子节点，二是只有右孩子
	// 晋升节点是从左子树找到的，有两种情况，一是叶子节点，二是只有左孩子
	if (_promotion_node->lchild == nullptr && _promotion_node->rchild == nullptr)
	{
		// 是叶子节点，直接晋升
		if (_promotion_node->parent->lchild == _promotion_node)
		{
			_promotion_node->parent->lchild = nullptr;
		}
		else
		{
			_promotion_node->parent->rchild = nullptr;
		}
	}
	else if (_promotion_node->lchild == nullptr && _promotion_node->rchild != nullptr)
	{
		_promotion_node->rchild->parent = _promotion_node->parent;
		_promotion_node->parent->rchild = _promotion_node->rchild;
	}
	else
	{
		_promotion_node->lchild->parent = _promotion_node->parent;
		_promotion_node->parent->lchild = _promotion_node->lchild;
	}
	_node->key = _promotion_node->key;
	delete _promotion_node;
	_promotion_node = nullptr;
	m_size--;
}

template<class _Ty>
void MyBSTree<_Ty>::preorder()
{
	preorder(root);
}

template<class _Ty>
void MyBSTree<_Ty>::preorder_iterative()
{
	MyBSTreeNode<_Ty>* tmp = root;
	MyStack<MyBSTreeNode<_Ty>*> stack(m_size);
	while (tmp || !stack.isEmpty())
	{
		while (tmp)
		{
			cout << tmp->key << " ";
			stack.push(tmp);
			tmp = tmp->lchild;
		}

		if (!stack.isEmpty())
		{
			tmp = stack.pop();
			tmp = tmp->rchild;
		}
	}
}

template<class _Ty>
void MyBSTree<_Ty>::inorder()
{
	inorder(root);
}

template<class _Ty>
void MyBSTree<_Ty>::inorder_iterative()
{
	MyBSTreeNode<_Ty>* tmp = root;
	MyStack<MyBSTreeNode<_Ty>*> stack(m_size);
	while (tmp || !stack.isEmpty())
	{
		while (tmp)
		{
			stack.push(tmp);
			tmp = tmp->lchild;
		}
		
		if (!stack.isEmpty())
		{
			tmp = stack.pop();
			cout << tmp->key << " ";
			tmp = tmp->rchild;
		}
	}
}

template<class _Ty>
void MyBSTree<_Ty>::postorder()
{
	postorder(root);
}

template<class _Ty>
void MyBSTree<_Ty>::postorder_iterative()
{
	MyBSTreeNode<_Ty>* tmp = root;
	MyStack<MyBSTreeNode<_Ty>*> stack(2 * m_size);

	// 如果节点非空或者栈非空，一直执行
	while (tmp || !stack.isEmpty())
	{
		// 当前节点非空，一直往左子树走
		while (tmp)
		{
			stack.push(tmp);	// 当前节点压栈
			stack.push(NULL);	// 压入nullptr作为标志位，表示tmp是否该出栈
			tmp = tmp->lchild;	// 继续往左子树走
		}

		// 走到最左节点，检查栈是否有元素
		if (!stack.isEmpty())
		{
			tmp = stack.pop();		// 栈顶元素出栈
			if (!tmp)				// 如果栈顶是nullptr，表示右子树还没遍历
			{
				tmp = stack.top();	// 获得栈顶元素，但不出栈
				tmp = tmp->rchild;	// 往右子树走
			}
			else					// 栈顶元素非空，表示右子树遍历完了
			{
				cout << tmp->key << " ";	// 访问子树根
				tmp = NULL;					// tmp置为空，因为下一步要把父节点的标志出栈
			}
		}
	}
}

template<class _Ty>
_Ty MyBSTree<_Ty>::min() const
{
	if (!root)
	{
		throw MyException("This is an empty binary tree with no elements.");
	}
	MyBSTreeNode<_Ty>* _root = root;
	_Ty _key;
	while (_root)
	{
		if (!_root->lchild)
		{
			_key = _root->key;
		}
		_root = _root->lchild;
	}
	return _key;
}

template<class _Ty>
_Ty MyBSTree<_Ty>::max() const
{
	if (!root)
	{
		throw MyException("This is an empty binary tree with no elements.");
	}
	MyBSTreeNode<_Ty>* _root = root;
	_Ty _key;
	while (_root)
	{
		if (!_root->rchild)
		{
			_key = _root->key;
		}
		_root = _root->rchild;
	}
	return _key;
}

template<class _Ty>
bool MyBSTree<_Ty>::contains(_Ty key) const
{
	if (search(key))
		return true;
	return false;
}

template<class _Ty>
bool MyBSTree<_Ty>::create(_Ty root_key, _Ty keys[], unsigned int size)
{
	if (this->root)
		return false;

	root = new MyBSTreeNode<_Ty>(root_key, nullptr, nullptr, nullptr);

	for (unsigned int i = 0; i < size; i++)
	{
		insert(keys[i]);
	}
}

template<class _Ty>
void MyBSTree<_Ty>::destroy()
{
	destroy(root);
	m_size = 0;
}

template<class _Ty>
inline int MyBSTree<_Ty>::size()
{
	return m_size;
}

template<class _Ty>
void MyBSTree<_Ty>::destroy(MyBSTreeNode<_Ty>* node)
{
	if (!node)
	{
		return;
	}

	if (!node->lchild && !node->rchild)
	{
		if (node->parent->lchild == node)
		{
			node->parent->lchild = nullptr;
		}
		else
		{
			node->parent->rchild = nullptr;
		}
		delete node;
		node = nullptr;
		return;
	}
	destroy(node->lchild);
	destroy(node->rchild);
	delete root;
	root = nullptr;
}

// 递归版本
template<class _Ty>
void MyBSTree<_Ty>::preorder(MyBSTreeNode<_Ty>* node)
{
	if (node != nullptr)
	{
		cout << node->key << " ";
		preorder(node->lchild);
		preorder(node->rchild);
	}
}

template<class _Ty>
void MyBSTree<_Ty>::inorder(MyBSTreeNode<_Ty>* node)
{
	if (node)
	{
		inorder(node->lchild);
		cout << node->key << " ";
		inorder(node->rchild);
	}
}

template<class _Ty>
void MyBSTree<_Ty>::postorder(MyBSTreeNode<_Ty>* node)
{
	if (node)
	{
		postorder(node->lchild);
		postorder(node->rchild);
		cout << node->key << " ";
	}
}

template<class _Ty>
MyBSTreeNode<_Ty>* MyBSTree<_Ty>::search(_Ty key) const
{
	MyBSTreeNode<_Ty>* _tmp = root;

	while (_tmp != nullptr && _tmp->key != key)
	{
		if (key < _tmp->key)
		{
			_tmp = _tmp->lchild;
		}
		else
		{
			_tmp = _tmp->rchild;
		}
	}
	return _tmp;
}
