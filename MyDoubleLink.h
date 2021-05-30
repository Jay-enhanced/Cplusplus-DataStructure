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
	MyDoubleLink() : count(0)
	{
		phead = new LinkNode<_Ty>();
		phead->prev = phead;
		phead->next = phead;
	}
	~MyDoubleLink()
	{
		LinkNode<_Ty>* ptmp;
		LinkNode<_Ty>* pnode = phead->next;

		while (pnode != phead)
		{
			ptmp = pnode;
			pnode = pnode->next;
			delete ptmp;
		}

		delete phead;
		phead = nullptr;
	}

	// ��������Ĵ�С
	unsigned int size() const
	{
		return count;
	}

	// ���������Ƿ�Ϊ��
	bool isEmpty() const
	{
		return count == 0;
	}

	// ��ͷ������һ���ڵ�
	void insert_front(_Ty value)
	{
		LinkNode<_Ty>* pnode = new LinkNode<_Ty>(value, phead, phead->next);
		phead->next->prev = pnode;
		phead->next = pnode;
		count++;
	}

	// ��β������һ���ڵ�
	void insert_tail(_Ty value)
	{
		LinkNode<_Ty>* pnode = new LinkNode<_Ty>(value, phead->prev, phead);
		phead->prev->next = pnode;
		phead->prev = pnode;
		count++;
	}

	// ��indexǰ���ӽڵ�
	void insert_at(const unsigned int& index, _Ty value)
	{
		if (index == 0)
		{
			insert_front(value);
			return;
		}

		LinkNode<_Ty>* pnode_index = get_node(index);
		LinkNode<_Ty>* pnode = new LinkNode<_Ty>(value, pnode_index->prev, pnode_index);
		pnode_index->prev->next = pnode;
		pnode_index->prev = pnode;
		count++;
	}

	// ɾ����һ��ֵΪvalue�Ľڵ�
	void delete_node(_Ty value)
	{
		if (isEmpty())
		{
			throw MyException("LinkList is empty.");
		}

		LinkNode<_Ty>* p = phead->next;
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

	// ͨ�������ֵ����ֵ�޸�һ���ڵ�
	void update_node(_Ty old_value, _Ty new_value)
	{
		LinkNode<_Ty>* p = phead->next;

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

	// ����index���Ľڵ�ֵ
	_Ty get(const unsigned int& index)
	{
		LinkNode<_Ty>* node = get_node(index);
		return node->value;
	}

private:
	unsigned int count;
	LinkNode<_Ty>* phead;
	LinkNode<_Ty>* get_node(int index)
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
};