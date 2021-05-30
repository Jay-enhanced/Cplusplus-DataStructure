#pragma once
#include "MyException.h"

template<class _Ty>
class MyStack
{
public:
	MyStack(int size)
	{
		this->size = size;
		stack = new _Ty[size];
		top = 0;
	}
	virtual ~MyStack()
	{
		delete[] stack;
	}

	void push(_Ty element)
	{
		if (isFull())
		{
			throw MyException("The stack space is full.");
		}
		stack[top++] = element;
	}
	_Ty pop(void)
	{
		top--;
		if (isEmpty())
			throw MyException("The stack is empty.");

		return stack[top];
	}
	_Ty peek(void)
	{
		top--;
		if (isEmpty())
			throw MyException("The stack is empty.");

		top++;
		return stack[top - 1];
	}

private:
	_Ty *stack;
	int  size;
	int  top;

	inline bool isEmpty(void) const
	{
		return top == -1;
	}
	inline bool isFull(void) const
	{
		return top == size;
	}
};