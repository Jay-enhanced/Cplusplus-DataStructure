#pragma once
#include <iostream>
using namespace std;

template <class T>
class Node
{
public:
    T value;
    Node* next;
    Node() : next(nullptr){}
    Node(T val, Node* p = nullptr) : value(val), next(p){}
};

template <class T>
class LinkList
{
private:
    int size;
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

    void push_back(const T& value);
    void append(const T& value);
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
    size = 0;
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
    size++;
}

template<class T>
void LinkList<T>::append(const T& value)
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
void LinkList<T>::print(ostream& os) const
{
    for (Node<T>* ptr = head; ptr != tail->next; ptr = ptr->next)
    {
        os << ptr->value << endl;
    }
}
