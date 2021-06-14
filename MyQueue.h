#pragma once

#include <iostream>
#include "MyException.h"

using namespace std;

template <class _Ty>
class MyQueue
{
public:
    MyQueue(unsigned int size);
    ~MyQueue();

    void add(_Ty element);
    _Ty front();
    _Ty pop();
    unsigned int size();
    inline bool isEmpty();
    inline bool isFull();

private:
    _Ty *queue;
    unsigned int m_counts;
    unsigned int m_size;
};

template <class _Ty>
MyQueue<_Ty>::MyQueue(unsigned int size)
{
    m_counts = 0;
    m_size = size;
    queue = new _Ty[size];
    if (!queue)
    {
        throw MyException("Create queue faile.");
    }
}

template <class _Ty>
MyQueue<_Ty>::~MyQueue()
{
    if (queue)
    {
        delete[] queue;
        queue = nullptr;
    }
}

template <class _Ty>
void MyQueue<_Ty>::add(_Ty element)
{
    if (!queue)
    {
        throw MyException("Queue is unavailable, add element faile");
    }
    if (isFull())
    {
        throw MyException("Queue is full, add element faile.");
    }

    queue[m_counts++] = element;
}

template <class _Ty>
_Ty MyQueue<_Ty>::front()
{
    if (isEmpty())
    {
        throw MyException("There is any elements in the queue.");
    }

    return queue[0];
}

template <class _Ty>
_Ty MyQueue<_Ty>::pop()
{
    int i = 0;

    if (isEmpty())
    {
        throw MyException("There is any elements in the queue.");
    }

    _Ty element = queue[0];
    m_counts--;

    while (i++ < m_counts)
    {
        queue[i - 1] = queue[i];
    }

    return element;
}

template <class _Ty>
unsigned int MyQueue<_Ty>::size()
{
    return m_size;
}

template <class _Ty>
bool MyQueue<_Ty>::isEmpty()
{
    return m_counts == 0;
}

template <class _Ty>
bool MyQueue<_Ty>::isFull()
{
    return m_counts == m_size;
}