#pragma once

#include "MyException.h"
#include <iostream>
#include <algorithm>
#include <iomanip>

using namespace std;

// AVL���ڵ�
template <class _Ty>
class MyAVLTreeNode
{
public:
    _Ty key;
    int height;
    MyAVLTreeNode* left;
    MyAVLTreeNode* right;
    MyAVLTreeNode* parent;

    MyAVLTreeNode(_Ty key, MyAVLTreeNode* left, MyAVLTreeNode* right, MyAVLTreeNode* parent)
    {
        this->key = key;
        this->height = 1;
        this->left = left;
        this->right = right;
        this->parent = parent;
    }
};

// AVL��
template <class _Ty>
class MyAVLTree
{
public:
	MyAVLTree();

    // 1.����ڵ�
    void insert(_Ty key);
    // 2.ɾ���ڵ�
    void remove(_Ty key);
    // 3.�ȸ�����
    void preOrder();
    // 4.�и�����
    void inOrder();
    // 5.�������
    void postOrder();
    // 6.��ȡ���ĸ߶�
    int height();
    // 7.��ȡ�������ֵ
    _Ty max();
    // 8.��ȡ������Сֵ
    _Ty min();
    // 9.����key�Ƿ�������
    bool contains(_Ty key);
    // 10.����AVL��
    void destroy();
    // 11.��ӡAVL��
    void print();

private:
    MyAVLTreeNode<_Ty>* root;

    MyAVLTreeNode<_Ty>* search_insert(_Ty key);
    MyAVLTreeNode<_Ty>* search_remove(_Ty key);
    int getHeight(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateLeft(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateRight(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateLeftRight(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* rotateRightLeft(MyAVLTreeNode<_Ty>* t);
    MyAVLTreeNode<_Ty>* backTracking(MyAVLTreeNode<_Ty>* t);
    void destroy(MyAVLTreeNode<_Ty>* t);
    void preorder(MyAVLTreeNode<_Ty>* t);
    void inorder(MyAVLTreeNode<_Ty>* t);
    void postorder(MyAVLTreeNode<_Ty>* t);
    void print(MyAVLTreeNode<_Ty>* t, _Ty key, int direction);
};

template<class _Ty>
MyAVLTree<_Ty>::MyAVLTree()
{
    root = nullptr;
}

template<class _Ty>
void MyAVLTree<_Ty>::insert(_Ty key)
{
    MyAVLTreeNode<_Ty>* pos = search_insert(key);
    MyAVLTreeNode<_Ty>* tmp = new MyAVLTreeNode<_Ty>(key, nullptr, nullptr, nullptr);
    if (!tmp)
    {
        throw MyException("�½��ڵ�ʧ�ܣ�\n");
    }

    // ���ڵ�Ϊ�գ�ֱ�Ӳ���
    if (!pos)
    {
        root = tmp;
        return;
    }

    if (key < pos->key) // �������
    {
        pos->left = tmp;
    }
    else if (key > pos->key)
    {
        pos->right = tmp;
    }
    else
        return;

    tmp->parent = pos;
    pos->height = std::max(getHeight(pos->left), getHeight(pos->right)) + 1;
    root = backTracking(pos);   // ����
}

template<class _Ty>
void MyAVLTree<_Ty>::remove(_Ty key)
{
    // ���ҵ�Ҫɾ���Ľڵ�
    MyAVLTreeNode<_Ty>* pos = search_remove(key);
    if (!pos)
    {
        return;
    }

    // �����Ҷ�ӽڵ㣬ֱ��ɾ�������Ӹ��ڵ㿪ʼ����
    if (!pos->left && !pos->right)
    {
        if (pos == root)
        {
            root->height = 0;
            delete root;
            root = nullptr;
            return;
        }

        MyAVLTreeNode<_Ty>* tmp = pos->parent;
        if (tmp->left == pos)    //�ýڵ��Ǹ��ڵ������
        {
            tmp->left = nullptr;
        }
        else
        {
            tmp->right = nullptr;
        }
        delete pos;
        pos = nullptr;
        tmp->height = std::max(getHeight(tmp->left), getHeight(tmp->right)) + 1;
        root = backTracking(tmp);
        return;
    }

    // ��ɾ���Ľڵ��Ƿ�Ҷ�ӽڵ㣬��Ҫ�������������ҵ���ֵ������key�Ľڵ���Ϊ�����ڵ�
    MyAVLTreeNode<_Ty>* _promotion_node = nullptr;
    if (pos->right)
    {
        // �������ǿգ����������ҵ����ߣ����������ĸ����ҽ����ڵ�
        MyAVLTreeNode<_Ty>* _tmp = pos->right;
        while (1)
        {
            if (_tmp->left)
            {
                _tmp = _tmp->left;
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
        // �������ǿգ����������ҵ��Ұ�ߣ����������ĸ����ҽ����ڵ�
        MyAVLTreeNode<_Ty>* _tmp = pos->left;
        while (1)
        {
            if (_tmp->right)
            {
                _tmp = _tmp->right;
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
    MyAVLTreeNode<_Ty>* back_track = _promotion_node->parent;
    if (!_promotion_node->left && !_promotion_node->right)   // Ҷ�ӽڵ�
    {
        // ��Ҷ�ӽڵ㣬ֱ�ӽ���
        if (_promotion_node->parent->left == _promotion_node)
            _promotion_node->parent->left = nullptr;
        else
            _promotion_node->parent->right = nullptr;
    }
    else if (!_promotion_node->left && _promotion_node->right)
    {
        _promotion_node->right->parent = _promotion_node->parent;
        _promotion_node->parent->right = _promotion_node->right;
    }
    else
    {
        _promotion_node->left->parent = _promotion_node->parent;
        _promotion_node->parent->left = _promotion_node->left;
    }

    pos->key = _promotion_node->key;
    delete _promotion_node;
    _promotion_node = nullptr;
    back_track->height = std::max(getHeight(back_track->left), getHeight(back_track->right)) + 1;
    root = backTracking(back_track);
}

template<class _Ty>
void MyAVLTree<_Ty>::preOrder()
{
    preorder(root);
}

template<class _Ty>
void MyAVLTree<_Ty>::inOrder()
{
    inorder(root);
}

template<class _Ty>
void MyAVLTree<_Ty>::postOrder()
{
    postorder(root);
}

template<class _Ty>
int MyAVLTree<_Ty>::height()
{
    if (!root)
        return 0;
    return root->height;
}

template<class _Ty>
_Ty MyAVLTree<_Ty>::max()
{
    MyAVLTreeNode<_Ty>* tmp = root;
    if (!tmp)
        throw MyException("����һ�ÿ������޷��ҵ����ֵ��\n");

    while (tmp)
    {
        if (tmp->right)
            tmp = tmp->right;
        else
            break;
    }

    return tmp->key;
}

template<class _Ty>
_Ty MyAVLTree<_Ty>::min()
{
    MyAVLTreeNode<_Ty>* tmp = root;
    if (!tmp)
        throw MyException("����һ�ÿ������޷��ҵ����ֵ��\n");

    while (tmp)
    {
        if (tmp->left)
            tmp = tmp->left;
        else
            break;
    }

    return tmp->key;
}

template<class _Ty>
bool MyAVLTree<_Ty>::contains(_Ty key)
{
    if (search_remove(key))
        return true;
    return false;
}

template<class _Ty>
void MyAVLTree<_Ty>::destroy()
{
    destroy(root);
    root = nullptr;
}

template<class _Ty>
void MyAVLTree<_Ty>::print()
{
    print(root, root->key, 0);
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::search_insert(_Ty key)
{
    MyAVLTreeNode<_Ty>* tmp = root;
    while (tmp)
    {
        if (key < tmp->key)
        {
            if (!tmp->left)
                return tmp;
            tmp = tmp->left;
        }            
        else if (key > tmp->key)
        {
            if (!tmp->right)
                return tmp;
            tmp = tmp->right;
        }
        else if(key == tmp->key)
            throw MyException("�����Ѿ����ڸýڵ㡣\n");
    }

    return tmp;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::search_remove(_Ty key)
{
    MyAVLTreeNode<_Ty>* tmp = root;
    while (tmp)
    {
        if (tmp->key == key)
            return tmp;
        else if (key < tmp->key)
            tmp = tmp->left;
        else
            tmp = tmp->right;
    }
    return tmp;
}

template<class _Ty>
int MyAVLTree<_Ty>::getHeight(MyAVLTreeNode<_Ty>* t)
{
    if (!t)
        return 0;
    return t->height;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateLeft(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->right;
    tmp->parent = t->parent;
    if (t->parent)
    {
        t->parent->right = tmp;
    }
    
    t->right = tmp->left;
    t->parent = tmp;
    tmp->left = t;
    t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;

    return tmp;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateRight(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->left;
    tmp->parent = t->parent;
    if (t->parent)
    {
        t->parent->left = tmp;
    }    
    t->left = tmp->right;
    t->parent = tmp;
    tmp->right = t;
    t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;

    return tmp;
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateLeftRight(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->left;
    tmp->left = tmp->right;
    tmp->right = nullptr;
    _Ty key = tmp->key;
    tmp->key = tmp->left->key;
    tmp->left->key = key;

    return rotateRight(t);
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::rotateRightLeft(MyAVLTreeNode<_Ty>* t)
{
    MyAVLTreeNode<_Ty>* tmp = t->right;
    tmp->right = tmp->left;
    tmp->left = nullptr;
    _Ty key = tmp->key;
    tmp->key = tmp->right->key;
    tmp->right->key = key;

    return rotateLeft(t);
}

template<class _Ty>
MyAVLTreeNode<_Ty>* MyAVLTree<_Ty>::backTracking(MyAVLTreeNode<_Ty>* t)
{
    // ��t�ĸ��ڵ㿪ʼ����
    MyAVLTreeNode<_Ty>* pback = t;
    MyAVLTreeNode<_Ty>* temp = pback->parent;
    while (temp != nullptr)
    {
        int lheight = getHeight(temp->left);
        int rheight = getHeight(temp->right);
        //temp->height = max(lheight, rheight) + 1;
        if ((lheight - rheight == 2) && temp->left->left)   // LL������
        {
            temp = rotateRight(temp);
        }
        else if ((lheight - rheight == 2) && temp->left->right) // LR��������������
        {
            temp = rotateLeftRight(temp);
        }
        else if ((lheight - rheight == -2) && temp->right->right) // RR������
        {
            temp = rotateLeft(temp);
        }
        else if ((lheight - rheight == -2) && temp->right->left) // RR��������������
        {
            temp = rotateRightLeft(temp);
        }
        pback = temp;
        temp = temp->parent;
    }
    pback->height = std::max(getHeight(pback->left), getHeight(pback->right)) + 1;
    return pback;
}

template<class _Ty>
void MyAVLTree<_Ty>::destroy(MyAVLTreeNode<_Ty>* t)
{
    if (!t)
        return;

    if(t->left)
        destroy(t->left);
    if (t->right)
        destroy(t->right);

    delete t;
}

template<class _Ty>
void MyAVLTree<_Ty>::preorder(MyAVLTreeNode<_Ty>* t)
{
    if (t)
    {
        cout << t->key << " ";
        preorder(t->left);
        preorder(t->right);
    }
}

template<class _Ty>
void MyAVLTree<_Ty>::inorder(MyAVLTreeNode<_Ty>* t)
{
    if (t)
    {
        inorder(t->left);
        cout << t->key << " ";
        inorder(t->right);
    }
}

template<class _Ty>
void MyAVLTree<_Ty>::postorder(MyAVLTreeNode<_Ty>* t)
{
    if (t)
    {
        inorder(t->left);        
        inorder(t->right);
        cout << t->key << " ";
    }
}

template<class _Ty>
void MyAVLTree<_Ty>::print(MyAVLTreeNode<_Ty>* t, _Ty key, int direction)
{
    if (t)
    {
        if (direction == 0)
            cout << setw(2) << t->key << "is root." << endl;
        else
            cout << setw(2) << t->key << "is " << setw(2) << key << "'s " << setw(12) << (direction == 1 ? "right child." : "left child.") << endl;

        print(t->left, t->key, -1);
        print(t->right, t->key, 1);
    }
}
