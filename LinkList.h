#pragma once
#include <iostream>
#include "MyException.h"
using namespace std;

template <class T>
class Node
{
public:
    T value;
    Node* next;
    Node() : next(nullptr) {}
    Node(T val, Node* p = nullptr) : value(val), next(p) {}
};

template <class T>
class LinkList
{
private:
    int m_size;
    Node<T>* head;
    Node<T>* tail;

    class List_Iterator
    {
    public:
        List_Iterator(Node<T>* p = nullptr) : ptr(p) {}

        Node<T>& operator*() const
        {
            return *ptr;
        }
        Node<T>* operator->() const
        {
            return ptr;
        }
        List_Iterator& operator++()
        {
            ptr = ptr->next;
            return *this;
        }

        List_Iterator operator++(int)
        {
            Node<T>* tmp = ptr;
            ++(*this);
            return List_Iterator(tmp);
        }

        bool operator==(const List_Iterator& t) const
        {
            return t.ptr == this->ptr;
        }

        bool operator!=(const List_Iterator& t) const
        {
            return t.ptr != this->ptr;
        }

    private:
        Node<T>* ptr;
    };

public:
    using iterator = List_Iterator;

    LinkList();
    ~LinkList();
    
    // 1.后插
    void push_back(const T& value);
    // 2.前插
    void insert_front(const T& value);
    // 3.在第一个值比value大的节点前插入
    void insert(const T& value);
    int  size();
    T min();
    T max();
    void print(ostream& os = cout) const;

    iterator begin() const
    {
        return List_Iterator(head);
    }
    iterator end() const
    {
        return List_Iterator(tail->next);
    }
};

template<class T>
LinkList<T>::LinkList()
{
    head = tail = nullptr;
    m_size = 0;
}

template<class T>
LinkList<T>::~LinkList()
{
    while (head)
    {
        Node<T>* tmp = head;
        head = head->next;
        delete tmp;
    }
    head = tail = nullptr;
}

template<class T>
void LinkList<T>::push_back(const T& value)
{
    if (!head)
    {
        head = new Node<T>(value);
        tail = head;
    }
    else
    {
        tail->next = new Node<T>(value);
        tail = tail->next;
    }
    m_size++;
}

template<class T>
void LinkList<T>::insert_front(const T& value)
{
    if (!head)
    {
        head = new Node<T>(value);
        tail = head;
    }
    else
    {
        Node<T>* node = new Node<T>(value, head);
        head = node;
    }
}

template<class T>
void LinkList<T>::insert(const T& value)
{
    Node<T>** tmp;

    for (tmp = &head; *tmp; tmp = &(*tmp)->next)
    {
        if ((*tmp)->value > value)
        {
            Node<T>* node = new Node<T>(value, *tmp);
            (*tmp) = node;
            return;
        }
    }
    (*tmp) = new Node<T>(value, nullptr);
    tail = *tmp;
}

template<class T>
inline int LinkList<T>::size()
{
    return m_size;
}

template<class T>
T LinkList<T>::min()
{
    Node<T>* tmp = head;
    if (!head)
        throw MyException("Linklist have any elements.\n");
    T min = head->value;
    while (tmp)
    {
        if (tmp->value < min)
            min = tmp->value;
        tmp = tmp->next;
    }
    return min;
}

template<class T>
T LinkList<T>::max()
{
    Node<T>* tmp = head;
    if (!head)
        throw MyException("Linklist have any elements.\n");
    T max = head->value;
    while (tmp)
    {
        if (tmp->value > max)
            max = tmp->value;
        tmp = tmp->next;
    }
    return max;
}

template<class T>
void LinkList<T>::print(ostream& os) const
{
    for (Node<T>* ptr = head; ptr != tail->next; ptr = ptr->next)
    {
        os << ptr->value << endl;
    }
}
