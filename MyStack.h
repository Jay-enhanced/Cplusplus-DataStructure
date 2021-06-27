#pragma once
#include "MyException.h"

template<class _Ty>
class MyStack
{
public:
	MyStack(int size);
	virtual ~MyStack();
	// 1.��ջ
	void push(_Ty element);
	// 2.��ջ
	_Ty pop(void);
	// 3.��ȡջ��Ԫ��
	_Ty top(void);
	// 4.���ջ�Ƿ�Ϊ��
	bool isEmpty(void) const;
	// 5.���ջ�Ƿ�����
	bool isFull(void) const;

private:
	_Ty *stack;
	int  size;
	int  m_top;
};

template<class _Ty>
MyStack<_Ty>::MyStack(int size)
{
	this->size = size;
	stack = new _Ty[size];
	m_top = 0;
}

template<class _Ty>
inline MyStack<_Ty>::~MyStack()
{
	delete[] stack;
}

template<class _Ty>
void MyStack<_Ty>::push(_Ty element)
{
	if (isFull())
	{
		throw MyException("The stack space is full.");
	}
	stack[m_top++] = element;
}

template<class _Ty>
_Ty MyStack<_Ty>::pop(void)
{
	if (isEmpty())
		throw MyException("The stack is empty.");

	return stack[--m_top];
}

template<class _Ty>
_Ty MyStack<_Ty>::top(void)
{
	if (isEmpty())
		throw MyException("The stack is empty.");

	return stack[m_top - 1];
}

template<class _Ty>
inline bool MyStack<_Ty>::isEmpty(void) const
{
	return m_top == 0;
}

template<class _Ty>
inline bool MyStack<_Ty>::isFull(void) const
{
	return m_top == size;
}
