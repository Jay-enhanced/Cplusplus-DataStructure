#pragma once

#include <iostream>
#include "MyException.h"

using namespace std;

template<class _Ty>
struct LinkNode
{
	_Ty value;
	LinkNode* prev;
	LinkNode* next;

	LinkNode(){}
	LinkNode(_Ty value, LinkNode* prev, LinkNode* next)
	{
		this->value = value;
		this->prev = prev;
		this->next = next;
	}
};

template<class _Ty>
class MyDoubleLink
{
public:
	using pNode = LinkNode<_Ty>*;
	using Node  = LinkNode<_Ty>;

	MyDoubleLink();
	~MyDoubleLink();

	// 1.��������Ĵ�С
	unsigned int size() const;
	// 2.���������Ƿ�Ϊ��
	bool isEmpty() const;
	// 3.ͷ��
	void insert_front(_Ty value);
	// 4.β��
	void insert_tail(_Ty value);
	// 5.��indexǰ���ӽڵ�
	void insert_at(const unsigned int& index, _Ty value);
	// 6.ɾ����һ��ֵΪvalue�Ľڵ�
	void delete_node(_Ty value);
	// 7.���½ڵ�
	void update_node(_Ty old_value, _Ty new_value);
	// 8.����index���Ľڵ�ֵ
	_Ty get(const unsigned int& index);
	// 9.��ӡ����
	void print();

	// ���õ�����
	class DoubleLinkIterator
	{
	public:
		DoubleLinkIterator(pNode p = nullptr):ptr(p){}
		// 1.����*�����
		Node& operator*() noexcept
		{
			return *ptr;
		}
		// 2.����->�����
		pNode operator->() noexcept
		{
			return ptr;
		}
		// 3.����==�����
		bool operator==(const DoubleLinkIterator& n) noexcept
		{
			return this->ptr == n.ptr;
		}
		// 4.����!=�����
		bool operator!=(const DoubleLinkIterator& n) noexcept
		{
			return this->ptr != n.ptr;
		}
		// 5.����ǰ��++�����
		DoubleLinkIterator& operator++() noexcept
		{
			ptr = ptr->next;
			return *this;
		}
		// 6.���غ���++�����
		DoubleLinkIterator operator++(int) noexcept
		{
			pNode tmp = ptr;
			ptr = ptr->next;
			return DoubleLinkIterator(tmp);
		}
		// 7.����ǰ��--�����
		DoubleLinkIterator& operator--() noexcept
		{
			ptr = ptr->prev;
			return *this;
		}
		// 8.���غ���--�����
		DoubleLinkIterator operator--(int) noexcept
		{
			pNode tmp = ptr;
			ptr = ptr->prev;
			return DoubleLinkIterator(tmp);
		}

	private:
		pNode ptr;
	};

	using iterator = DoubleLinkIterator;

	iterator begin()
	{
		return DoubleLinkIterator(phead->next);
	}
	iterator end()
	{
		return DoubleLinkIterator(phead);
	}

private:
	unsigned int count;
	pNode phead;
	LinkNode<_Ty>* get_node(int index);
};

template<class _Ty>
MyDoubleLink<_Ty>::MyDoubleLink():count(0)
{
	phead = new LinkNode<_Ty>();
	phead->prev = phead;
	phead->next = phead;
}

template<class _Ty>
MyDoubleLink<_Ty>::~MyDoubleLink()
{
	phead->prev->next = nullptr;
	while (phead)
	{
		pNode tmp = phead;
		phead = phead->next;
		delete tmp;
	}
	phead = nullptr;
}

template<class _Ty>
inline unsigned int MyDoubleLink<_Ty>::size() const
{
	return count;
}

template<class _Ty>
inline bool MyDoubleLink<_Ty>::isEmpty() const
{
	return count == 0;
}

template<class _Ty>
void MyDoubleLink<_Ty>::insert_front(_Ty value)
{
	pNode pnode = new Node(value, phead, phead->next);
	phead->next->prev = pnode;
	phead->next = pnode;
	count++;
}

template<class _Ty>
void MyDoubleLink<_Ty>::insert_tail(_Ty value)
{
 	pNode pnode = new Node(value, phead->prev, phead);
	phead->prev->next = pnode;
	phead->prev = pnode;
	count++;
}

template<class _Ty>
void MyDoubleLink<_Ty>::insert_at(const unsigned int& index, _Ty value)
{
	if (index == 0)
	{
		insert_front(value);
		return;
	}

	pNode pnode_index = get_node(index);
	pNode pnode = new Node(value, pnode_index->prev, pnode_index);
	pnode_index->prev->next = pnode;
	pnode_index->prev = pnode;
	count++;
}

template<class _Ty>
void MyDoubleLink<_Ty>::delete_node(_Ty value)
{
	if (isEmpty())
	{
		throw MyException("LinkList is empty.");
	}

	pNode p = phead->next;
	while (p != phead)
	{
		if (p->value == value)
		{
			p->prev->next = p->next;
			p->next->prev = p->prev;
			delete p;
			count--;
			return;
		}
		p = p->next;
	}
	throw MyException("The corresponding node was not found in the LinkList.");
}

template<class _Ty>
void MyDoubleLink<_Ty>::update_node(_Ty old_value, _Ty new_value)
{
	pNode p = phead->next;

	while (p != phead)
	{
		if (p->value == old_value)
		{
			p->value = new_value;
			return;
		}
		p = p->next;
	}
	throw MyException("The corresponding node was not found in the LinkList.");
}

template<class _Ty>
_Ty MyDoubleLink<_Ty>::get(const unsigned int& index)
{
	pNode node = get_node(index);
	return node->value;
}

template<class _Ty>
void MyDoubleLink<_Ty>::print()
{
	pNode tmp = phead;
	while (tmp && tmp->next != phead)
	{
		cout << tmp->next->value << " ";
		tmp = tmp->next;
	}
}

template<class _Ty>
inline LinkNode<_Ty>* MyDoubleLink<_Ty>::get_node(int index)
{
	if (index < 0 || index > count - 1 || count == 0)
	{
		throw MyException("Index out of range.");
	}

	// �������
	if (index <= count / 2)
	{
		unsigned int i = 0;
		LinkNode<_Ty>* p_index = phead->next;
		while (i++ < index)
		{
			p_index = p_index->next;
		}
		return p_index;
	}

	// �������
	unsigned int i = 0;
	unsigned int counts = count - index - 1;
	LinkNode<_Ty>* r_index = phead->prev;
	while (i++ < counts)
	{
		r_index = r_index->prev;
	}
	return r_index;
}
