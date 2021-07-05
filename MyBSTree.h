#pragma once
#include "MyException.h"
#include "MyStack.h"
#include <iostream>

using namespace std;

// �ڵ�
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

// ������
template <class _Ty>
class MyBSTree
{
public:
	MyBSTree();
	~MyBSTree();

	// 1.�����ֵΪkey�Ľڵ�
	void insert(_Ty key);
	// 2.ɾ����ֵΪkey�Ľڵ�
	void remove(_Ty key);
	// 3.�ȸ�����
	void preorder();			// �ݹ�汾
	void preorder_iterative();	// �����汾
	// 4.�и�����
	void inorder();				// �ݹ�汾
	void inorder_iterative();	// �����汾
	// 5.�������
	void postorder();			// �ݹ�汾
	void postorder_iterative();	// �����汾
	// 6.���ض������е���Сֵ
	_Ty min() const;
	// 7.���ض������е����ֵ
	_Ty max() const;
	// 8.���Ҽ�ֵkey�Ƿ��ڶ�������
	bool contains(_Ty key) const;
	// 9.����������
	bool create(_Ty root_key, _Ty keys[], unsigned int size);
	// 10.���ٶ�����
	void destroy();
	// 11.�������Ĵ�С
	int size();

private:
	// ˽�г�Ա�ͷ��������ڲ�ʹ��
	MyBSTreeNode<_Ty>* root;
	int m_size;
	// 1.���Ҽ�ֵΪkey�Ľڵ�
	MyBSTreeNode<_Ty>* search(_Ty key) const;
	// 2.�ȸ�����
	void preorder(MyBSTreeNode<_Ty>* node);
	// 3.�и�����
	void inorder(MyBSTreeNode<_Ty>* node);
	// 4.�������
	void postorder(MyBSTreeNode<_Ty>* node);
	// 5.���ٶ�����
	void destroy(MyBSTreeNode<_Ty>* node);
};

// ���캯��
template <class _Ty>
MyBSTree<_Ty>::MyBSTree()
{
	m_size = 0;
	root = nullptr;
}

// ��������
template <class _Ty>
MyBSTree<_Ty>::~MyBSTree()
{
	destroy();
}

// 1.�����ֵΪkey�Ľڵ�
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

// 2.ɾ����ֵΪkey�Ľڵ�
template<class _Ty>
void MyBSTree<_Ty>::remove(_Ty key)
{
	MyBSTreeNode<_Ty>* _node = search(key);
	if (_node == nullptr)
	{
		throw MyException("The node is not found and can not be deleted.");
	}

	// ��ɾ���Ľڵ���Ҷ�ӽڵ㣬ֱ��ɾ��
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

	// ��ɾ���Ľڵ��Ƿ�Ҷ�ӽڵ㣬��Ҫ�������������ҵ���ֵ������key�Ľڵ���Ϊ������ϲ�������ڵ�
	MyBSTreeNode<_Ty>* _promotion_node = nullptr;
	if (_node->rchild != nullptr)
	{
		// �������ǿգ����������ҵ����ߣ����������ĸ����ҽ�����ϲ�������ڵ�
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
		// �������ǿյģ���Ҫ�����������Ұ�ߣ����������ĸ����ҽ����ڵ�
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

	// �����ڵ��Ǵ��������ҵ��ģ������������һ��Ҷ�ӽڵ㣬����ֻ���Һ���
	// �����ڵ��Ǵ��������ҵ��ģ������������һ��Ҷ�ӽڵ㣬����ֻ������
	if (_promotion_node->lchild == nullptr && _promotion_node->rchild == nullptr)
	{
		// ��Ҷ�ӽڵ㣬ֱ�ӽ���
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

	// ����ڵ�ǿջ���ջ�ǿգ�һֱִ��
	while (tmp || !stack.isEmpty())
	{
		// ��ǰ�ڵ�ǿգ�һֱ����������
		while (tmp)
		{
			stack.push(tmp);	// ��ǰ�ڵ�ѹջ
			stack.push(NULL);	// ѹ��nullptr��Ϊ��־λ����ʾtmp�Ƿ�ó�ջ
			tmp = tmp->lchild;	// ��������������
		}

		// �ߵ�����ڵ㣬���ջ�Ƿ���Ԫ��
		if (!stack.isEmpty())
		{
			tmp = stack.pop();		// ջ��Ԫ�س�ջ
			if (!tmp)				// ���ջ����nullptr����ʾ��������û����
			{
				tmp = stack.top();	// ���ջ��Ԫ�أ�������ջ
				tmp = tmp->rchild;	// ����������
			}
			else					// ջ��Ԫ�طǿգ���ʾ��������������
			{
				cout << tmp->key << " ";	// ����������
				tmp = NULL;					// tmp��Ϊ�գ���Ϊ��һ��Ҫ�Ѹ��ڵ�ı�־��ջ
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

// �ݹ�汾
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
